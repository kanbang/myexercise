
// ExpandDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExpandDlg.h"
#include "ExpandDlgDlg.h"

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


// CExpandDlgDlg �Ի���




CExpandDlgDlg::CExpandDlgDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CExpandDlgDlg::IDD, pParent )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CExpandDlgDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CExpandDlgDlg, CDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED( IDC_BUTTON2, &CExpandDlgDlg::OnBnClickedButton2 )
    ON_BN_CLICKED( IDC_BUTTON3, &CExpandDlgDlg::OnBnClickedButton3 )
END_MESSAGE_MAP()


// CExpandDlgDlg ��Ϣ��������

BOOL CExpandDlgDlg::OnInitDialog()
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

    m_childDlg.Create( IDD_DIALOG1, this );
    //SetWindowLong(m_childDlg.GetSafeHwnd(), GWL_STYLE, WS_CHILD | WS_VISIBLE);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExpandDlgDlg::OnSysCommand( UINT nID, LPARAM lParam )
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

void CExpandDlgDlg::OnPaint()
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
HCURSOR CExpandDlgDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}


void CExpandDlgDlg::OnBnClickedButton2()
{
    CRect rect;
    GetDlgItem( IDC_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );

    CString msg;
    msg.Format( _T( "left:%d, right:%d, top:%d, bottom:%d, width:%d, height:%d" ), rect.left, rect.right, rect.top, rect.bottom, rect.Width(), rect.Height() );

    CRect dlgRect;
    m_childDlg.GetWindowRect( &dlgRect );
    msg.AppendFormat( _T( "\nleft:%d, right:%d, top:%d, bottom:%d, width:%d, height:%d" ), dlgRect.left, dlgRect.right, dlgRect.top, dlgRect.bottom, dlgRect.Width(), dlgRect.Height() );

    ScreenToClient( &dlgRect );
    msg.AppendFormat( _T( "\n[C]left:%d, right:%d, top:%d, bottom:%d, width:%d, height:%d" ), dlgRect.left, dlgRect.right, dlgRect.top, dlgRect.bottom, dlgRect.Width(), dlgRect.Height() );

    MessageBox( msg );

    dlgRect.left = rect.left;
    dlgRect.top = rect.bottom;
    dlgRect.right += rect.left;
    dlgRect.bottom += rect.bottom;

    CRect thisDlgRect;
    GetWindowRect( &thisDlgRect );
    thisDlgRect.bottom += dlgRect.Height();
    SetWindowPos( NULL, 0, 0, thisDlgRect.Width(), thisDlgRect.Height(), SWP_NOMOVE | SWP_NOZORDER );

    m_childDlg.MoveWindow( &dlgRect );
    m_childDlg.ShowWindow( SW_SHOW );
}

#include "TestDlg.h"

void CExpandDlgDlg::OnBnClickedButton3()
{
    TestDlg dlg;
    dlg.DoModal();
}