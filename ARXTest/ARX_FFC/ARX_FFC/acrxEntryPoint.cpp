#include "StdAfx.h"
#include "resource.h"

// ����ע���������
#ifndef ARX_FFC_SERVICE_NAME
#define ARX_FFC_SERVICE_NAME _T("ARX_FFC_SERVICE_NAME")
#endif

#include "GasParamHelper.h"
#include "GambitParamHelper.h"
#include "ReactorHelper.h"
#include "UIHelper.h"

class CARX_FFCApp : public AcRxArxApp
{

public:
    CARX_FFCApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // ע�����
        acrxRegisterService( ARX_FFC_SERVICE_NAME );

        ReactorHelper::CreateSelectedGE_EditorReactor();
        UIHelper::CreateDataDlgManager();

        AfxInitRichEdit();

        acutPrintf( _T( "\nARX_FFC::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ������
        delete acrxServiceDictionary->remove( ARX_FFC_SERVICE_NAME );

        ReactorHelper::RemoveSelectedGE_EditorReactor();
        UIHelper::DestroyDataDlgManager();

        acutPrintf( _T( "\nARX_FFC::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // ��ʼ����˹����
        GasParamHelper::InitParams();
        // ��ʼ��gambit���ν�ģ����
        GambitParamHelper::InitParams();

        /* ȫ�ֺ���(RegFields.cpp) */
        extern void RegFields();

        /* ȫ�ֺ���(InitDefData.cpp) */
        extern void InitDefData();

        // ע���ֶ�
        RegFields();

        // ��ʼ��Ĭ������
        InitDefData();

        acutPrintf( _T( "\nARX_FFC::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nARX_FFC::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void JL_ShowFFCDlg()
    {
        UIHelper::ShowFFCDlg();
    }

    static void JL_ShowGoafGasDlg()
    {
        UIHelper::ShowGoafGasDlg();
    }

    static void JL_SwitchMonitorPickFirst()
    {
        ReactorHelper::SwitchMonitorPickFirst();
    }

    static void JL_ShowGambitParamDlg()
    {
        UIHelper::ShowGambitParamDlg();
    }

    //static void JL_testaa()
    //{
    //	extern void lgqMyJigdragLArc();
    //	lgqMyJigdragLArc();
    //}
} ;

IMPLEMENT_ARX_ENTRYPOINT( CARX_FFCApp )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_FFCApp, JL, _ShowFFCDlg, ShowFFCDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_FFCApp, JL, _ShowGoafGasDlg, ShowGoafGasDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_FFCApp, JL, _SwitchMonitorPickFirst, SwitchMonitorPickFirst, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_FFCApp, JL, _ShowGambitParamDlg, ShowGambitParamDlg, ACRX_CMD_TRANSPARENT, NULL )

//ACED_ARXCOMMAND_ENTRY_AUTO(CARX_FFCApp, JL, _testaa, testaa, ACRX_CMD_TRANSPARENT, NULL)
