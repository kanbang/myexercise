
// SwitchDlgTestDlg.cpp : 实现文件
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
    // 创建对话框对象
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

// SwitchDlg 消息处理程序

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持

// 实现
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


// CSwitchDlgTestDlg 对话框




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


// CSwitchDlgTestDlg 消息处理程序

BOOL CSwitchDlgTestDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
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

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon( m_hIcon, TRUE );			// 设置大图标
    SetIcon( m_hIcon, FALSE );		// 设置小图标

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

    mpos = 0;  // 默认显示第1个对话框

    da[mpos]->ShowWindow( SW_SHOW );

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSwitchDlgTestDlg::OnPaint()
{
    if ( IsIconic() )
    {
        CPaintDC dc( this ); // 用于绘制的设备上下文

        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rect;
        GetClientRect( &rect );
        int x = ( rect.Width() - cxIcon + 1 ) / 2;
        int y = ( rect.Height() - cyIcon + 1 ) / 2;

        // 绘制图标
        dc.DrawIcon( x, y, m_hIcon );
    }
    else
    {
        CDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
        pDlg->DestroyWindow();  // 销毁对话框资源
        delete pDlg;            // 删除对话框对象内存
    }
}