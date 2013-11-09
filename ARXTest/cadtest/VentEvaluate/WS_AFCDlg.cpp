#include "stdafx.h"
#include "WS_AFCDlg.h"

#include "MCDlg.h"
#include "GasDlg.h"
#include "MpnwDlg.h"

IMPLEMENT_DYNAMIC( WS_AFCDlg, CDialog )

WS_AFCDlg::WS_AFCDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( WS_AFCDlg::IDD, pParent )
{

}

WS_AFCDlg::~WS_AFCDlg()
{
}

void WS_AFCDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( WS_AFCDlg, CDialog )
    ON_BN_CLICKED( IDC_WS_AFC_MC_BTN, &WS_AFCDlg::OnBnClickedAfcMcBtn )
    ON_BN_CLICKED( IDC_WS_AFC_GAS_BTN, &WS_AFCDlg::OnBnClickedAfcGasBtn )
    ON_BN_CLICKED( IDC_WS_AFC_MPNW_BTN, &WS_AFCDlg::OnBnClickedAfcMpnwBtn )
END_MESSAGE_MAP()


BOOL WS_AFCDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if( !m_lowGasMine )
    {
        GetDlgItem( IDC_WS_AFC_MC_BTN )->EnableWindow( FALSE );
    }
    else
    {
        if( m_inHighGas )
        {
            GetDlgItem( IDC_WS_AFC_MC_BTN )->EnableWindow( FALSE );
        }
        else
        {
            GetDlgItem( IDC_WS_AFC_GAS_BTN )->EnableWindow( FALSE );
        }
    }

    return TRUE;
}

void WS_AFCDlg::OnBnClickedAfcMcBtn()
{
    CAcModuleResourceOverride myResources;

    MCDlg dlg;
    dlg.m_objId = m_objId;
    dlg.DoModal();
}

void WS_AFCDlg::OnBnClickedAfcGasBtn()
{
    CAcModuleResourceOverride myResources;

    GasDlg dlg;
    dlg.m_objId = m_objId;
    dlg.gas_type = GT_WS;
    dlg.DoModal();
}

void WS_AFCDlg::OnBnClickedAfcMpnwBtn()
{
    CAcModuleResourceOverride myResources;

    MpnwDlg dlg;
    dlg.m_objId = m_objId;
    dlg.DoModal();
}
