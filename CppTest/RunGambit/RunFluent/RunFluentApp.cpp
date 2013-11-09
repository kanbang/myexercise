
// RunFluent.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RunFluentApp.h"
#include "RunFluentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRunFluentApp

BEGIN_MESSAGE_MAP( CRunFluentApp, CWinAppEx )
    ON_COMMAND( ID_HELP, &CWinApp::OnHelp )
END_MESSAGE_MAP()


// CRunFluentApp ����

CRunFluentApp::CRunFluentApp()
{
    // TODO: �ڴ˴����ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRunFluentApp ����

CRunFluentApp theApp;


// CRunFluentApp ��ʼ��

BOOL CRunFluentApp::InitInstance()
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

    CWinAppEx::InitInstance();

    AfxEnableControlContainer();

    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey( _T( "Ӧ�ó��������ɵı���Ӧ�ó���" ) );

    CRunFluentDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if ( nResponse == IDOK )
    {
        // TODO: �ڴ˷��ô�����ʱ��
        //  ��ȷ�������رնԻ���Ĵ���
    }
    else if ( nResponse == IDCANCEL )
    {
        // TODO: �ڴ˷��ô�����ʱ��
        //  ��ȡ�������رնԻ���Ĵ���
    }

    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    //  ����������Ӧ�ó������Ϣ�á�
    return FALSE;
}