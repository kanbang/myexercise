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


// CircularJointDraw_ConfigDlg 消息处理程序

BOOL CircularJointDraw_ConfigDlg::OnInitDialog()
{
    //SetDialogName("AsdkAcUiSample:AsdkAcUiDialog");
    CAcUiDialog::OnInitDialog();

    // 显示已有的半径数据
    m_radiusEdit.SetWindowText( m_strRadius );
    m_radiusEdit.Convert();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CircularJointDraw_ConfigDlg::OnEnKillfocusRadiusEdit()
{
    m_radiusEdit.Convert();
    m_radiusEdit.GetWindowText( m_strRadius );
}

void CircularJointDraw_ConfigDlg::OnBnClickedOk()
{
    // 正常情况下应该进行一些检查工作
    // 目前就暂时不处理了
    OnOK();
}
