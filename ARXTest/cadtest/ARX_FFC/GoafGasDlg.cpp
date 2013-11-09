#include "stdafx.h"
#include "GoafGasDlg.h"

#include "GasParamHelper.h"

IMPLEMENT_DYNAMIC( GoafGasDlg, CDialog )

GoafGasDlg::GoafGasDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( GoafGasDlg::IDD, pParent )
    , m_goaf_gas( 0 )
{

}

GoafGasDlg::~GoafGasDlg()
{
}

void GoafGasDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_GOAF_GAS_EDIT, m_goaf_gas );
    DDX_Text( pDX, IDC_SIDE_GAS_EDIT, m_side_gas );
    DDX_Text( pDX, IDC_CUT_GAS_EDIT, m_cut_gas );
    DDX_Text( pDX, IDC_TOP_GAS_EDIT, m_top_gas );
    DDX_Text( pDX, IDC_BOTTOM_GAS_EDIT, m_bottom_gas );
    DDV_MinMaxDouble( pDX, m_goaf_gas, 0, 10000 );
    DDV_MinMaxDouble( pDX, m_side_gas, 0, 10000 );
    DDV_MinMaxDouble( pDX, m_cut_gas, 0, 10000 );
    DDV_MinMaxDouble( pDX, m_top_gas, 0, 10000 );
    DDV_MinMaxDouble( pDX, m_bottom_gas, 0, 10000 );
}


BEGIN_MESSAGE_MAP( GoafGasDlg, CDialog )
    ON_BN_CLICKED( IDOK, &GoafGasDlg::OnBnClickedOk )
END_MESSAGE_MAP()


// GoafGasDlg 消息处理程序

void GoafGasDlg::OnBnClickedOk()
{
    if( UpdateData( TRUE ) )
    {
        GasParam gp;
        gp.f1 = m_goaf_gas;
        gp.f2 = m_side_gas;
        gp.f3 = m_cut_gas;
        gp.f4 = m_top_gas;
        gp.f5 = m_bottom_gas;

        GasParamHelper::WriteGasParam( gp );

        OnOK();
    }
}

BOOL GoafGasDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    GasParam gp;
    GasParamHelper::ReadGasParam( gp );

    m_goaf_gas = gp.f1;
    m_side_gas = gp.f2;
    m_cut_gas = gp.f3;
    m_top_gas = gp.f4;
    m_bottom_gas = gp.f5;

    UpdateData( FALSE );

    return TRUE;
}
