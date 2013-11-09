#include "stdafx.h"
#include "DoubleTunnelDraw_ConfigDlg.h"

IMPLEMENT_DYNAMIC( DoubleTunnelDraw_ConfigDlg, CAcUiDialog )

DoubleTunnelDraw_ConfigDlg::DoubleTunnelDraw_ConfigDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( DoubleTunnelDraw_ConfigDlg::IDD, pParent )
{
    m_jdt = 0;
}

DoubleTunnelDraw_ConfigDlg::~DoubleTunnelDraw_ConfigDlg()
{
}

void DoubleTunnelDraw_ConfigDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_WIDTH_EDIT, m_widthEdit );
    DDX_Control( pDX, IDC_JOINT_DRAW_TYPE_LIST, m_jointDrawList );
}


BEGIN_MESSAGE_MAP( DoubleTunnelDraw_ConfigDlg, CDialog )
    ON_EN_KILLFOCUS( IDC_WIDTH_EDIT, &DoubleTunnelDraw_ConfigDlg::OnEnKillfocusWidthEdit )
    ON_BN_CLICKED( IDOK, &DoubleTunnelDraw_ConfigDlg::OnBnClickedOk )
END_MESSAGE_MAP()


// DoubleTunnelDraw_ConfigDlg ��Ϣ�������

void DoubleTunnelDraw_ConfigDlg::OnEnKillfocusWidthEdit()
{
    m_widthEdit.Convert();
    m_widthEdit.GetWindowText( m_strWidth );
}

void DoubleTunnelDraw_ConfigDlg::OnBnClickedOk()
{
    m_jdt = m_jointDrawList.GetCurSel();

    OnOK();
}

BOOL DoubleTunnelDraw_ConfigDlg::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();

    // ��ʼ����ʾ��ǰ���
    m_widthEdit.SetWindowText( m_strWidth );
    m_widthEdit.Convert();

    m_jointDrawList.AddString( _T( "��" ) );
    m_jointDrawList.AddString( _T( "ʮ�ֽ���Բ" ) );
    m_jointDrawList.AddString( _T( "���ʵ��Բ" ) );
    m_jointDrawList.SetCurSel( m_jdt );

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
