#include "stdafx.h"
#include "TWS_AFCDlg.h"

#include "GasDlg.h"
#include "LocalFanDlg.h"
#include "MpnwDlg.h"

IMPLEMENT_DYNAMIC( TWS_AFCDlg, CDialog )

TWS_AFCDlg::TWS_AFCDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( TWS_AFCDlg::IDD, pParent )
{

}

TWS_AFCDlg::~TWS_AFCDlg()
{
}

void TWS_AFCDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}


BEGIN_MESSAGE_MAP( TWS_AFCDlg, CDialog )
    ON_BN_CLICKED( IDC_TWS_AFC_GAS_BTN, &TWS_AFCDlg::OnBnClickedTwsAfcGasBtn )
    ON_BN_CLICKED( IDC_TWS_AFC_LOCAL_FAN_BTN, &TWS_AFCDlg::OnBnClickedTwsAfcLocalFanBtn )
    ON_BN_CLICKED( IDC_TWS_AFC_MPNW_BTN, &TWS_AFCDlg::OnBnClickedTwsAfcMpnwBtn )
END_MESSAGE_MAP()


// TWS_AFCDlg ��Ϣ�������

BOOL TWS_AFCDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void TWS_AFCDlg::OnBnClickedTwsAfcGasBtn()
{
    CAcModuleResourceOverride myResources;

    GasDlg dlg;
    dlg.m_objId = m_objId;
    dlg.gas_type = GT_TWS;
    dlg.DoModal();
}

void TWS_AFCDlg::OnBnClickedTwsAfcLocalFanBtn()
{
    CAcModuleResourceOverride myResources;

    LocalFanDlg dlg;
    dlg.m_objId = m_objId;
    dlg.DoModal();
}

void TWS_AFCDlg::OnBnClickedTwsAfcMpnwBtn()
{
    CAcModuleResourceOverride myResources;

    MpnwDlg dlg;
    dlg.m_objId = m_objId;
    dlg.DoModal();
}
