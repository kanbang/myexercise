
// RunFluentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RunFluentApp.h"
#include "RunFluentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV ֧��

// ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog( CAboutDlg::IDD )
{
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialog )
END_MESSAGE_MAP()


// CRunFluentDlg �Ի���




CRunFluentDlg::CRunFluentDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CRunFluentDlg::IDD, pParent )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CRunFluentDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CRunFluentDlg, CDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED( IDC_BUTTON1, &CRunFluentDlg::OnBnClickedButton1 )
    ON_BN_CLICKED( IDC_BUTTON2, &CRunFluentDlg::OnBnClickedButton2 )
    ON_BN_CLICKED( IDC_BUTTON3, &CRunFluentDlg::OnBnClickedButton3 )
    ON_BN_CLICKED( IDC_BUTTON4, &CRunFluentDlg::OnBnClickedButton4 )
    ON_BN_CLICKED( IDC_BUTTON5, &CRunFluentDlg::OnBnClickedButton5 )
END_MESSAGE_MAP()


// CRunFluentDlg ��Ϣ��������

BOOL CRunFluentDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��������...���˵������ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
    ASSERT( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
    ASSERT( IDM_ABOUTBOX < 0xF000 );

    CMenu* pSysMenu = GetSystemMenu( FALSE );
    if ( pSysMenu != NULL )
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString( IDS_ABOUTBOX );
        ASSERT( bNameValid );
        if ( !strAboutMenu.IsEmpty() )
        {
            pSysMenu->AppendMenu( MF_SEPARATOR );
            pSysMenu->AppendMenu( MF_STRING, IDM_ABOUTBOX, strAboutMenu );
        }
    }

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon( m_hIcon, TRUE );			// ���ô�ͼ��
    SetIcon( m_hIcon, FALSE );		// ����Сͼ��

    // TODO: �ڴ����Ӷ���ĳ�ʼ������

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRunFluentDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
    if ( ( nID & 0xFFF0 ) == IDM_ABOUTBOX )
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand( nID, lParam );
    }
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRunFluentDlg::OnPaint()
{
    if ( IsIconic() )
    {
        CPaintDC dc( this ); // ���ڻ��Ƶ��豸������

        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rect;
        GetClientRect( &rect );
        int x = ( rect.Width() - cxIcon + 1 ) / 2;
        int y = ( rect.Height() - cyIcon + 1 ) / 2;

        // ����ͼ��
        dc.DrawIcon( x, y, m_hIcon );
    }
    else
    {
        CDialog::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRunFluentDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}

#include "FFC_Def.h"
#include "SysTool.h"
extern bool RunFluent( const CString& appDir );
extern bool RunGambit( const CString& appDir );
extern bool RunTecplot( const CString& appDir, int type );

void CRunFluentDlg::OnBnClickedButton1()
{
    CString appDir = GetAppPathDir();
    if( RunFluent( appDir ) )
    {
        MessageBox( _T( "����Fluent�ɹ�" ) );
    }
    else
    {
        MessageBox( _T( "����Fluentʧ��" ) );
    }
}

void CRunFluentDlg::OnBnClickedButton2()
{
    CString appDir = GetAppPathDir();
    if( RunGambit( appDir ) )
    {
        MessageBox( _T( "����Gambit�ɹ�" ) );
    }
    else
    {
        MessageBox( _T( "����Gambitʧ��" ) );
    }
}

void CRunFluentDlg::OnBnClickedButton3()
{
    CString appDir = GetAppPathDir();

    if( RunTecplot( appDir, TST_VELOCITY_NEPHOGRAM ) )
    {
        MessageBox( _T( "����Tecplot�ɹ�" ) );
    }
    else
    {
        MessageBox( _T( "����Tecplotʧ��" ) );
    }
}

void CRunFluentDlg::OnBnClickedButton4()
{
    //KillProcess(_T("notepad.exe"));
    KillProcess( _T( "gambit.exe" ) );
    KillProcess( _T( "exceed.exe" ) );
    KillProcess( _T( "fluent.exe" ) );
    KillProcess( _T( "fl6326s.exe" ) );
}

static void xxx()
{
    //TCHAR varName[] = _T("Path");
    TCHAR varName[] = _T( "testxx" );

    HKEY hkUser;
    if( ERROR_SUCCESS != RegOpenCurrentUser( KEY_ALL_ACCESS, &hkUser ) )
    {
        //AfxMessageBox(_T())
        return;
    }

    HKEY hkResult;
    if( ERROR_SUCCESS != RegOpenKeyEx( hkUser, _T( "Environment" ), 0, KEY_ALL_ACCESS, &hkResult ) )
    {
        AfxMessageBox( _T( "����ע���������" ) );
        return;
    }

    DWORD BufferSize = 8192;
    DWORD cbData;
    DWORD dwRet;

    VOID* PerfData = malloc( BufferSize );
    cbData = BufferSize;

    dwRet = RegQueryValueEx( hkResult,
                             varName,
                             NULL,
                             NULL,
                             ( LPBYTE ) PerfData,
                             &cbData );

    while ( dwRet == ERROR_MORE_DATA )
    {
        BufferSize += 4096;
        PerfData = realloc( PerfData, BufferSize );
        cbData = BufferSize;

        dwRet = RegQueryValueEx( hkResult,
                                 varName,
                                 NULL,
                                 NULL,
                                 ( LPBYTE ) PerfData,
                                 &cbData );

    }

    AfxMessageBox( ( LPCTSTR )PerfData );

    LPTSTR javaHome = _T( ";ddd" );

    TCHAR* temp = new TCHAR[cbData + sizeof( javaHome )];

    _tcscpy_s( temp, cbData, ( LPTSTR )PerfData );

    LPTSTR path = _tcscat( temp, javaHome );

    RegSetValueEx( hkResult,
                   varName,
                   0,
                   REG_EXPAND_SZ,                // REG_SZ
                   ( LPBYTE ) path,
                   _tcsclen( path )*sizeof( TCHAR ) );

    DWORD dwResult;

    SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0, LPARAM( "Environment" ), SMTO_ABORTIFHUNG, 5000, &dwResult );

    delete temp;
}

void CRunFluentDlg::OnBnClickedButton5()
{
    //TCHAR name[] = _T("Path");
    //TCHAR value1[1000];

    //DWORD dwRet = GetEnvironmentVariable(name, value1, 1000);

    //TCHAR name2[] = _T("myvar");
    //TCHAR value2[100];

    //SetEnvironmentVariable(name2, _T("d:\\xxx"));
    ///*DWORD dwRet = */GetEnvironmentVariable(name2, value2, 100);

    //MessageBox(value2);

    xxx();
}