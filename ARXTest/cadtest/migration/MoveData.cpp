#include "stdafx.h"
#include "HelperClass.h"
#include "config.h"
#include "MineGE.h"

static void GetTypeNames( const AcDbObjectIdArray& objIds, AcStringArray& types )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        pTrans->getObject( pObj, objIds[i], AcDb::kForRead );

        MineGE* pGE = MineGE::cast( pObj );
        types.append( pGE->getTypeName() );
    }
    actrTransactionManager->endTransaction();
}

static void GetDataObjects( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& dbObjIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        pTrans->getObject( pObj, objIds[i], AcDb::kForRead );

        MineGE* pGE = MineGE::cast( pObj );

        AcDbObjectId dbObjId = pGE->getDataObject();
        dbObjIds.append( dbObjId );
    }
    actrTransactionManager->endTransaction();
}

static void MoveOneData( const AcDbObjectId& objId, const AcDbObjectId& dbObjId )
{
    AcDbObject* pObj;
    acdbOpenObject( pObj, objId, AcDb::kForWrite );

    resbuf* pAppNode = pObj->xData( _T( "临时数据" ) );
    if( pAppNode != 0 )
    {
        resbuf* pTemp = pAppNode;
        while( pTemp->rbnext != 0 )
        {
            pTemp = pTemp->rbnext;
            CString name = pTemp->resval.rstring;
            pTemp = pTemp->rbnext;
            CString value = pTemp->resval.rstring;

            bool ret = DataHelper::SetPropertyData( dbObjId, name, value );

            if( ret )
            {
                acutPrintf( _T( "\n字段:%s\t值:%s" ), name, value );
            }
        }

        pTemp = pAppNode->rbnext;
        acutRelRb( pTemp );

        pAppNode->rbnext = 0;
        pObj->setXData( pAppNode ); // 删除扩展数据
        acutRelRb( pAppNode );
    }

    pObj->close();
}

static void MoveData_Helper( const AcDbObjectIdArray& objIds,
                             const AcDbObjectIdArray& dbObjIds )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        MoveOneData( objIds[i], dbObjIds[i] );
    }
}

static void RemoveAppName( AcDbDatabase* db, const CString& appName )
{
    // make sure application name is registered
    AcDbRegAppTable* regAppTbl;
    db->getSymbolTable( regAppTbl, AcDb::kForWrite );
    AcDbRegAppTableRecord* pRegAppRecord;
    regAppTbl->getAt( appName, pRegAppRecord, AcDb::kForWrite );
    regAppTbl->close();

    pRegAppRecord->erase( true );
    pRegAppRecord->close();
}

void MoveData()
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MineGE" ), objIds );

    AcDbObjectIdArray dbObjIds;
    GetDataObjects( objIds, dbObjIds );

    MoveData_Helper( objIds, dbObjIds );

    RemoveAppName( acdbHostApplicationServices()->workingDatabase(), _T( "临时数据" ) );
}