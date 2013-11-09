#include "StdAfx.h"

#include "config.h"
#include "MVSSData.h"

#include "../ArxHelper/HelperClass.h"

// �����ݿ��л�ȡĳ�����������ֶ�
static void GetFields( CDaoDatabase* pDB, MVSS_DATA_TYPE type, AcStringArray& fields )
{
    CDaoTableDef tbl( pDB );
    tbl.Open( GetTableName( type ) );

    int n = tbl.GetFieldCount();
    for( int i = 0; i < n; i++ )
    {
        CDaoFieldInfo fi;
        tbl.GetFieldInfo( i, fi );
        fields.append( fi.m_strName );
    }

    tbl.Close();
}

static void DeleteDict( const CString& dictName )
{
    AcDbDictionary* pNameObjDict;
    acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary( pNameObjDict, AcDb::kForWrite );

    pNameObjDict->remove( dictName );

    pNameObjDict->close();
}

static void AddFields( CDaoDatabase* pDB, MVSS_DATA_TYPE type )
{
    // ��ȡ�ֶ�
    AcStringArray fields;
    GetFields( pDB, type, fields );

    // д���ֶ�
    ArxDictTool* pDcit = ArxDictTool::GetDictTool( VNG_FIELD_DICT );
    int n = fields.length();
    for( int i = 0; i < n; i++ )
    {
        pDcit->addEntry( GetTypeName( type ), fields[i].kACharPtr() );
    }
    delete pDcit;
}

void InitDataField( const CString& dbPath, const CString& pwd )
{
    // ��մʵ�
    DeleteDict( VNG_FIELD_DICT );

    // ����ע��ʵ�
    ArxDataTool::RegDict( VNG_FIELD_DICT );

    // �����ݿ��ж�ȡ�ֶ�
    CDaoDatabase db;

    // ���������ַ���
    CString cnt;
    if( pwd.GetLength() != 0 )
    {
        cnt.Format( _T( ";PWD=%s" ), pwd ); // ����
    }

    try
    {
        db.Open( dbPath, 0, 0, cnt );

        AddFields( &db, MDT_NODE );
        AddFields( &db, MDT_EDGE );
        AddFields( &db, MDT_GATE );
        AddFields( &db, MDT_FAN );

        db.Close();
    }
    catch ( CDaoException* e )
    {
        e->ReportError();
        e->Delete();

        if( db.IsOpen() ) db.Close();
    }
}