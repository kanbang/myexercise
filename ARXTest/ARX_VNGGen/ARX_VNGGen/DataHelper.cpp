#include "stdafx.h"

#include "config.h"
#include "MVSSData.h"

#include "../VNG_GE/VNG_GE.h"

#include "../ArxHelper/HelperClass.h"

/* 实现在ARX_VNGGen.cpp */
extern void GetAllFields( const CString& type, AcStringArray& fields );

static void GetIds( const AcDbObjectIdArray& objIds, AcDbIntArray& ids )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        VNG_GE* pGE = VNG_GE::cast( pObj );
        ids.append( pGE->m_id );
    }

    actrTransactionManager->endTransaction();
}

static int ReadId( const CString& data, MVSS_DATA_TYPE type )
{
    MVSSData* pData = CreateMVSSData( type );
    pData->readData( data );
    int id = pData->id;
    delete pData;

    return id;
}

static void WriteDataToObject( const AcDbObjectId& objId, const AcStringArray& values )
{
    AcDbObject* pObj;
    if( Acad::eOk != acdbOpenObject( pObj, objId, AcDb::kForWrite ) ) return;

    resbuf* pAppNode = pObj->xData( VNG_XDATA_GROUP );
    if( pAppNode == 0 )
    {
        pAppNode = acutBuildList( AcDb::kDxfRegAppName, VNG_XDATA_GROUP, 0 );
    }
    // 遍历到最后
    resbuf* pTemp = pAppNode;
    while( pTemp->rbnext != 0 )
    {
        pTemp = pTemp->rbnext;
    }

    // 添加数据
    int len = values.length();
    for( int i = 0; i < len; i++ )
    {
        pTemp->rbnext = acutBuildList( AcDb::kDxfXdAsciiString, values[i].kACharPtr(), 0 );
        while( pTemp->rbnext != 0 )
        {
            pTemp = pTemp->rbnext;
        }
    }
    pObj->setXData( pAppNode );
    acutRelRb( pAppNode );

    pObj->close();
}

static void GetObjectData( CDaoRecordset& rs, const AcStringArray& fields, AcStringArray& values )
{
    int n = fields.length();
    for( int i = 0; i < n; i++ )
    {
        COleVariant v;
        rs.GetFieldValue( fields[i].kACharPtr(), v );
        if( v.vt == VT_NULL )
        {
            values.append( _T( "" ) );
        }
        else
        {
            v.ChangeType( VT_BSTR );
            values.append( v.bstrVal );
        }
    }
}

static void WriteAllPropertyDatas( CDaoDatabase* pDB,
                                   MVSS_DATA_TYPE type,
                                   const AcStringArray& fields,
                                   const AcDbObjectIdArray& objIds,
                                   const AcDbIntArray& ids )
{
    CDaoRecordset rs( pDB );
    CString sql;
    sql.Format( _T( "select * from %s" ), GetTableName( type ) );
    rs.Open( dbOpenDynaset, sql, 0 );

    rs.MoveFirst () ;
    while( !rs.IsEOF() )
    {
        CString data;
        if( ReadComment( rs, data ) )
        {
            int pos = ids.find( ReadId( data, type ) );
            if( pos >= 0 )
            {
                // 读取数据
                AcStringArray values;
                GetObjectData( rs, fields, values );
                // 写入数据
                WriteDataToObject( objIds[pos], values );
            }
        }
        rs.MoveNext ();
    }

    rs.Close();
}

static void ReadWriteData( CDaoDatabase* pDB, MVSS_DATA_TYPE type )
{
    AcStringArray fields;
    GetAllFields( GetTypeName( type ), fields );
    if( fields.isEmpty() ) return;

    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( GetTypeName( type ), objIds );
    if( objIds.isEmpty() ) return;

    AcDbIntArray ids;
    GetIds( objIds, ids );

    WriteAllPropertyDatas( pDB, type, fields, objIds, ids );
}

void ReadWriteAllDatas( const CString& dbPath, const CString& pwd )
{
    // 注册扩展数据服务名
    ArxDataTool::RegAppName( acdbHostApplicationServices()->workingDatabase(), VNG_XDATA_GROUP );

    CDaoDatabase db;

    // 构造连接字符串
    CString cnt;
    if( pwd.GetLength() != 0 )
    {
        cnt.Format( _T( ";PWD=%s" ), pwd ); // 密码
    }

    try
    {
        db.Open( dbPath, 0, 0, cnt );

        ReadWriteData( &db, MDT_NODE ); // 写入节点数据
        ReadWriteData( &db, MDT_EDGE ); // 写入分支数据
        ReadWriteData( &db, MDT_GATE ); // 写入构筑物数据
        ReadWriteData( &db, MDT_FAN ); // 写入通风动力数据

        db.Close();
    }
    catch ( CDaoException* e )
    {
        e->ReportError();
        e->Delete();

        if( db.IsOpen() ) db.Close();
    }
}