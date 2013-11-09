#include "StdAfx.h"
#include "resource.h"

#include "DataCmd.h"
#include "UIHelper.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

// ����ע���������
#ifndef MINEGE_CMDS_SERVICE_NAME
#define MINEGE_CMDS_SERVICE_NAME _T("MINEGECMDS_SERVICE_NAME")
#endif

// ��������ģ��
class CMineGECmdsApp : public AcRxArxApp
{

public:
    CMineGECmdsApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( MINEGE_CMDS_SERVICE_NAME );

        acutPrintf( _T( "\nMineGECmds::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ������
        delete acrxServiceDictionary->remove( MINEGE_CMDS_SERVICE_NAME );

        acutPrintf( _T( "\nMineGECmds::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

        acutPrintf( _T( "\nMineGECmds::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        acutPrintf( _T( "\nMineGECmds::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void JL_CopyPropertyData( void )
    {
        DataCmd::CopyData();;
    }

    static void JL_CopyPropertyData2( void )
    {
        DataCmd::CopyData2();
    }

    static void JL_ConfigVisualDraw()
    {
        UIHelper::ShowDrawManagerDlg();
    }

    static void JL_ConfigDataField()
    {
        UIHelper::ShowFieldManagerDlg();
    }

    static void JL_DisplayData()
    {
        UIHelper::DisplayData();
    }

    static void JL_DisplayDataByDoubleClick()
    {
        UIHelper::DisplayDataByDoubleClick();
    }

    static void JL_ConfigVarList()
    {
        UIHelper::ShowVarListDlg();
    }

    static void JL_DisplayDataObject()
    {
        UIHelper::ShowDataObjectStateDlg();
    }

    static void JL_ShowAllTagGE()
    {
        AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��ͼԪ:" ) );
        if( objId.isNull() ) return;
        if( !ArxUtilHelper::IsEqualType( _T( "MineGE" ), objId ) ) return;

        DrawHelper::ShowAllTagGE( objId, 2 ); // �û���ɫ������ʾ
    }

    static void JL_ShowHostGE()
    {
        AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ����ǩͼԪ:" ) );
        if( objId.isNull() ) return;
        if( !ArxUtilHelper::IsEqualType( _T( "TagGE" ), objId ) ) return;

        DrawHelper::ShowHostGE( objId, 2 ); // �û���ɫ������ʾ
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CMineGECmdsApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _CopyPropertyData, CopyPropertyData, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _CopyPropertyData2, CopyPropertyData2, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ConfigVisualDraw, ConfigVisualDraw, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ConfigDataField, ConfigDataField, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DisplayData, DisplayData, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DisplayDataByDoubleClick, DisplayDataByDoubleClick, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ConfigVarList, ConfigVarList, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DisplayDataObject, DisplayDataObject, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ShowAllTagGE, ShowAllTagGE, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ShowHostGE, ShowHostGE, ACRX_CMD_TRANSPARENT, NULL )
