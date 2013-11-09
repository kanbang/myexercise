
// SwitchDlgTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwitchDlgTest.h"
#include "SwitchDlgTestDlg.h"

#include "ChildDlg1.h"
#include "ChildDlg2.h"
#include "ChildDlg3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CDialog* CreateChildDlg( int flag, long id, CWnd* pParent )
{
    // �����Ի������
    CDialog* pDlg = 0;
    switch( flag )
    {
    case 1:
        pDlg = new ChildDlg1();
        break;
    case 2:
        pDlg = new ChildDlg2();
        break;
    case 3:
        pDlg = new ChildDlg3();
        break;
    }

    pDlg->Create( id, pParent );

    return pDlg;
}

// SwitchDlg ��Ϣ�������

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


// CSwitchDlgTestDlg �Ի���




CSwitchDlgTestDlg::CSwitchDlgTestDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CSwitchDlgTestDlg::IDD, pParent )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CSwitchDlgTestDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CSwitchDlgTestDlg, CDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED( IDC_PRE_BTN, &CSwitchDlgTestDlg::OnBnClickedPreBtn )
    ON_BN_CLICKED( IDC_NEXT_BTN, &CSwitchDlgTestDlg::OnBnClickedNextBtn )
END_MESSAGE_MAP()


// CSwitchDlgTestDlg ��Ϣ�������

BOOL CSwitchDlgTestDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

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

    da.push_back( CreateChildDlg( 1, IDD_DIALOG1, this ) ); // ChildDlg1
    da.push_back( CreateChildDlg( 2, IDD_DIALOG2, this ) ); // ChildDlg2
    da.push_back( CreateChildDlg( 3, IDD_DIALOG3, this ) ); // ChildDlg3

    CRect rect ;
    GetDlgItem( IDC_DLG_POS )->GetWindowRect( &rect );
    GetDlgItem( IDC_DLG_POS )->SetWindowText( "" );
    ScreenToClient( &rect );
    for( int i = 0; i < da.size(); i++ )
    {
        da[i]->MoveWindow( &rect, TRUE );
    }

    mpos = 0;  // Ĭ����ʾ��1���Ի���

    da[mpos]->ShowWindow( SW_SHOW );

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSwitchDlgTestDlg::OnSysCommand( UINT nID, LPARAM lParam )
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSwitchDlgTestDlg::OnPaint()
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
HCURSOR CSwitchDlgTestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}


void CSwitchDlgTestDlg::switchDlg( int pos )
{
    for( int i = 0; i < ( int )da.size(); i++ )
    {
        int nCmdShow = ( ( pos == i ) ? SW_SHOW : SW_HIDE );
        da[i]->ShowWindow( nCmdShow );
    }
}

void CSwitchDlgTestDlg::OnBnClickedPreBtn()
{
    if( mpos > 0 )
    {
        switchDlg( mpos );
        mpos--;
        if( mpos >= 0 )
        {
            switchDlg( mpos );
        }
    }
}

void CSwitchDlgTestDlg::OnBnClickedNextBtn()
{
    if( mpos < da.size() )
    {
        switchDlg( mpos );
        mpos++;
        if( mpos < da.size() )
        {
            switchDlg( mpos );
        }
    }
}

CSwitchDlgTestDlg::~CSwitchDlgTestDlg()
{
    for( int i = 0; i < da.size(); i++ )
    {
        CDialog* pDlg = da[i];
        pDlg->DestroyWindow();  // ���ٶԻ�����Դ
        delete pDlg;            // ɾ���Ի�������ڴ�
    }
}