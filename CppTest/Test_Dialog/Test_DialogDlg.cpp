// Test_DialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test_Dialog.h"
#include "Test_DialogDlg.h"
#include "Private.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTest_DialogDlg 对话框




CTest_DialogDlg::CTest_DialogDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CTest_DialogDlg::IDD, pParent )
    , m_edit1( 0 )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
    m_cstest = _T( "chudshihua" );
}

void CTest_DialogDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_COMBO1, m_cmb_color );
    DDX_Control( pDX, IDC_SPIN, m_spintest );
    DDX_Control( pDX, IDC_EDIT, m_edit );
    DDX_Text( pDX, IDC_EDIT, m_edit1 );
}

BEGIN_MESSAGE_MAP( CTest_DialogDlg, CDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED( IDOK, &CTest_DialogDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDTEST, &CTest_DialogDlg::OnBnClickedTest )
    ON_NOTIFY( UDN_DELTAPOS, IDC_SPIN, &CTest_DialogDlg::OnDeltaposSpin )
    ON_EN_UPDATE( IDC_EDIT, &CTest_DialogDlg::OnEnUpdateEdit )
    ON_BN_CLICKED( ID_DIALOG, &CTest_DialogDlg::OnBnClickedDialog )
END_MESSAGE_MAP()


// CTest_DialogDlg 消息处理程序

BOOL CTest_DialogDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
    ASSERT( IDM_ABOUTBOX < 0xF000 );

    CMenu* pSysMenu = GetSystemMenu( FALSE );
    if ( pSysMenu != NULL )
    {
        CString strAboutMenu;
        strAboutMenu.LoadString( IDS_ABOUTBOX );
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

    // TODO: 在此添加额外的初始化代码
    m_cmb_color.AddItem( _T( "淡紫" ), RGB( 202, 111, 255 ) );
    m_cmb_color.AddItem( _T( "淡蓝" ), RGB( 202, 224, 245 ) );
    m_cmb_color.AddItem( _T( "天蓝" ), RGB( 68, 159, 245 ) );
    m_cmb_color.AddItem( _T( "红色" ), RGB( 255, 0, 0 ) );
    m_cmb_color.AddItem( _T( "蓝色" ), RGB( 0, 0, 255 ) );
    m_cmb_color.AddItem( _T( "绿色" ), RGB( 0, 255, 0 ) );
    m_cmb_color.AddItem( _T( "黄色" ), RGB( 255, 255, 0 ) );
    m_cmb_color.AddItem( _T( "粉色" ), RGB( 255, 0, 255 ) );
    m_cmb_color.AddItem( _T( "棕色" ), RGB( 255, 128, 64 ) );
    m_cmb_color.AddItem( _T( "其它" ), RGB( 213, 233, 249 ) );
    m_cmb_color.SetCurSel( 0 );

    ////////////////////////////test spinbuttonCtrl
    m_spintest.SetBuddy( GetDlgItem( IDC_EDIT ) ); //设置伙伴窗口
    m_spintest.SetRange( 0, 255 ); //设置min~max
    m_spintest.SetPos( 122 ); //设置起始位置
    m_spintest.SetBase( 3 );







    m_edit1 = 9;


    UpdateData( FALSE );




    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTest_DialogDlg::OnSysCommand( UINT nID, LPARAM lParam )
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

void CTest_DialogDlg::OnPaint()
{
    if ( IsIconic() )
    {
        CPaintDC dc( this ); // 用于绘制的设备上下文

        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );

        // 使图标在工作矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CTest_DialogDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}


void CTest_DialogDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    MessageBox( _T( "btn ok" ) );
    m_cstest = _T( "btn 0k" );


}
CString CTest_DialogDlg::Get()
{
    return m_cstest;
}
void CTest_DialogDlg::OnBnClickedTest()

{

}

void CTest_DialogDlg::OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    CDialog::OnDrawItem( nIDCtl, lpDrawItemStruct );
}
void CTest_DialogDlg::OnDeltaposSpin( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>( pNMHDR );
    // TODO: 在此添加控件通知处理程序代码

    *pResult = 0;
}

void CTest_DialogDlg::OnEnUpdateEdit()
{
    // TODO:  如果该控件是 RICHEDIT 控件，则它将不会
    // 发送该通知，除非重写 CDialog::OnInitDialog()
    // 函数，将 EM_SETEVENTMASK 消息发送到控件，
    // 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

    // TODO:  在此添加控件通知处理程序代码
}

void CTest_DialogDlg::OnBnClickedDialog()
{
    // TODO: 在此添加控件通知处理程序代码
    CPrivate dlg;
    dlg.DoModal();
}
