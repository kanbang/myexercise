#include "StdAfx.h"
#include "resource.h"

// ����ע���������
#ifndef OBJECT_RECORD_POOL_SERVICE_NAME
#define OBJECT_RECORD_POOL_SERVICE_NAME _T("OBJECTRECORDPOOL_SERVICE_NAME")
#endif

#include "ObjectRecordPool.h"

#include "../ArxHelper/HelperClass.h"

static void ZoomToEntity( const AcDbObjectId& objId )
{
    ads_name en;
    if( Acad::eOk != acdbGetAdsName( en, objId ) ) return;
    acedCommand( RTSTR, _T( "ZOOM" ), RTSTR, _T( "O" ), RTENAME, en, RTSTR, _T( "" ), 0 );
}

class CObjectRecordPoolApp : public AcRxArxApp
{

public:
    CObjectRecordPoolApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( OBJECT_RECORD_POOL_SERVICE_NAME );
        //acutPrintf(_T("\nObjectRecordPool::On_kInitAppMsg\n"));

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ������
        delete acrxServiceDictionary->remove( OBJECT_RECORD_POOL_SERVICE_NAME );
        //acutPrintf(_T("\nObjectRecordPool::On_kUnloadAppMsg\n"));

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // ���������¼��
        ORPHelper::CreateORP();
        //acutPrintf(_T("\nObjectRecordPool::On_kLoadDwgMsg\n"));

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        // ���ٶ����¼��
        ORPHelper::DeleteORP();

        //acutPrintf(_T("\nObjectRecordPool::On_kUnloadDwgMsg\n"));

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void ORP_EnableORP()
    {
        bool flag = ORPHelper::getORPState();
        acutPrintf( _T( "\n�л�ǰ״̬:%d" ), ( int )flag );
        ORPHelper::SetORPState( !flag );

        flag = ORPHelper::getORPState();
        acutPrintf( _T( "\n�л���״̬:%d" ), ( int )flag );
    }

    static void ORP_GetNextObject()
    {
        acutPrintf( _T( "\n����ظ���:%d" ), ORPHelper::GetCount() );
        AcDbObjectId objId;
        if( ORPHelper::GetObject( objId ) )
        {
            acutPrintf( _T( "\n��ȡʵ��ɹ�" ) );
            ZoomToEntity( objId );
        }
    }

    static void ORP_test()
    {
        AcDbObjectIdArray objIds;
        ArxDataTool::GetEntsByType( _T( "AcDbEntity" ), objIds );

        int n = objIds.length();
        for( int i = 0; i < n; i++ )
        {
            ORPHelper::AddObject( objIds[i] );
        }
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CObjectRecordPoolApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CObjectRecordPoolApp, ORP, _EnableORP, EnableORP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CObjectRecordPoolApp, ORP, _GetNextObject, GetNextObject, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CObjectRecordPoolApp, ORP, _test, test, ACRX_CMD_TRANSPARENT, NULL )
