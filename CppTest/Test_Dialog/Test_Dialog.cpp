// Test_Dialog.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Test_Dialog.h"
#include "Test_DialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest_DialogApp

BEGIN_MESSAGE_MAP( CTest_DialogApp, CWinApp )
    ON_COMMAND( ID_HELP, &CWinApp::OnHelp )
END_MESSAGE_MAP()


// CTest_DialogApp ����

CTest_DialogApp::CTest_DialogApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTest_DialogApp ����

CTest_DialogApp theApp;


// CTest_DialogApp ��ʼ��

BOOL CTest_DialogApp::InitInstance()
{
    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof( InitCtrls );
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx( &InitCtrls );

    CWinApp::InitInstance();

    if ( !AfxSocketInit() )
    {
        AfxMessageBox( IDP_SOCKETS_INIT_FAILED );
        return FALSE;
    }

    AfxEnableControlContainer();

    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey( _T( "Ӧ�ó��������ɵı���Ӧ�ó���" ) );

    CTest_DialogDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if ( nResponse == IDOK )
    {
        // TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
        //  �Ի���Ĵ���
    }
    else if ( nResponse == IDCANCEL )
    {
        // TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
        //  �Ի���Ĵ���
    }

    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    //  ����������Ӧ�ó������Ϣ�á�
    return FALSE;
}
