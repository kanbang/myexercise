#include "StdAfx.h"

#include "../ArxHelper/HelperClass.h"
#include "../VNG_GE/VNG_Edge.h"

/* 实现在ARX_VNGGen.cpp */
extern void GetAllFields( const CString& type, AcStringArray& fields );
extern void GetAllDatas( const AcDbObjectId& objId, AcStringArray& values );

static void GetEdgeDatas( const AcDbObjectIdArray& objIds, int index, AcStringArray& values )
{
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcStringArray t_values;
        GetAllDatas( objIds[i], t_values );
        values.append( t_values[index] );
    }
}

static bool GetEdgeNames( const AcDbObjectIdArray& objIds, AcStringArray& names )
{
    AcStringArray fields;
    GetAllFields( _T( "VNG_Edge" ), fields );
    int pos = fields.find( _T( "名称" ) );
    if( pos == -1 ) return false;

    GetEdgeDatas( objIds, pos, names );
    return true;
}

void WriteNameToEdge()
{
    // 查找所有的分支
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "VNG_Edge" ), objIds );

    // 获取分支的名称数据
    AcStringArray names;
    if( !GetEdgeNames( objIds, names ) ) return;

    // 写入分支数据
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        VNG_Edge* pEdge = VNG_Edge::cast( pObj );
        pEdge->setName( names[i].kACharPtr() );
    }
    actrTransactionManager->endTransaction();
}