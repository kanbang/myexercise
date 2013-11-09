#include "stdafx.h"
#include "CircularJointDraw_ConfigDlg.h"

IMPLEMENT_DYNAMIC( CircularJointDraw_ConfigDlg, CAcUiDialog )

CircularJointDraw_ConfigDlg::CircularJointDraw_ConfigDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( CircularJointDraw_ConfigDlg::IDD, pParent )
{

}

CircularJointDraw_ConfigDlg::~CircularJointDraw_ConfigDlg()
{
}

void CircularJointDraw_ConfigDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_RADIUS_EDIT, m_radiusEdit );
}


BEGIN_MESSAGE_MAP( CircularJointDraw_ConfigDlg, CDialog )
    ON_EN_KILLFOCUS( IDC_RADIUS_EDIT, &CircularJointDraw_ConfigDlg::OnEnKillfocusRadiusEdit )
    ON_BN_CLICKED( IDOK, &CircularJointDraw_ConfigDlg::OnBnClickedOk )
END_MESSAGE_MAP()


// CircularJointDraw_ConfigDlg ��Ϣ�������

BOOL CircularJointDraw_ConfigDlg::OnInitDialog()
{
    //SetDialogName("AsdkAcUiSample:AsdkAcUiDialog");
    CAcUiDialog::OnInitDialog();

    // ��ʾ���еİ뾶����
    m_radiusEdit.SetWindowText( m_strRadius );
    m_radiusEdit.Convert();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CircularJointDraw_ConfigDlg::OnEnKillfocusRadiusEdit()
{
    m_radiusEdit.Convert();
    m_radiusEdit.GetWindowText( m_strRadius );
}

void CircularJointDraw_ConfigDlg::OnBnClickedOk()
{
    // ���������Ӧ�ý���һЩ��鹤��
    // Ŀǰ����ʱ��������
    OnOK();
}
