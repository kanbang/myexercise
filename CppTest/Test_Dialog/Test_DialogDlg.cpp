// Test_DialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test_Dialog.h"
#include "Test_DialogDlg.h"
#include "Private.h"

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


// CTest_DialogDlg �Ի���




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


// CTest_DialogDlg ��Ϣ�������

BOOL CTest_DialogDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon( m_hIcon, TRUE );			// ���ô�ͼ��
    SetIcon( m_hIcon, FALSE );		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_cmb_color.AddItem( _T( "����" ), RGB( 202, 111, 255 ) );
    m_cmb_color.AddItem( _T( "����" ), RGB( 202, 224, 245 ) );
    m_cmb_color.AddItem( _T( "����" ), RGB( 68, 159, 245 ) );
    m_cmb_color.AddItem( _T( "��ɫ" ), RGB( 255, 0, 0 ) );
    m_cmb_color.AddItem( _T( "��ɫ" ), RGB( 0, 0, 255 ) );
    m_cmb_color.AddItem( _T( "��ɫ" ), RGB( 0, 255, 0 ) );
    m_cmb_color.AddItem( _T( "��ɫ" ), RGB( 255, 255, 0 ) );
    m_cmb_color.AddItem( _T( "��ɫ" ), RGB( 255, 0, 255 ) );
    m_cmb_color.AddItem( _T( "��ɫ" ), RGB( 255, 128, 64 ) );
    m_cmb_color.AddItem( _T( "����" ), RGB( 213, 233, 249 ) );
    m_cmb_color.SetCurSel( 0 );

    ////////////////////////////test spinbuttonCtrl
    m_spintest.SetBuddy( GetDlgItem( IDC_EDIT ) ); //���û�鴰��
    m_spintest.SetRange( 0, 255 ); //����min~max
    m_spintest.SetPos( 122 ); //������ʼλ��
    m_spintest.SetBase( 3 );







    m_edit1 = 9;


    UpdateData( FALSE );




    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTest_DialogDlg::OnPaint()
{
    if ( IsIconic() )
    {
        CPaintDC dc( this ); // ���ڻ��Ƶ��豸������

        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );

        // ʹͼ���ڹ��������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CTest_DialogDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}


void CTest_DialogDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    *pResult = 0;
}

void CTest_DialogDlg::OnEnUpdateEdit()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
    // ���͸�֪ͨ��������д CDialog::OnInitDialog()
    // �������� EM_SETEVENTMASK ��Ϣ���͵��ؼ���
    // ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CTest_DialogDlg::OnBnClickedDialog()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CPrivate dlg;
    dlg.DoModal();
}
