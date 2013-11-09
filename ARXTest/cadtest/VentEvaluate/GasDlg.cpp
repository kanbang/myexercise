#include "stdafx.h"
#include "GasDlg.h"

#include "../MineGE/HelperClass.h"
#include "../MineGE/DataLink.h"

class GasDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "平均绝对瓦斯涌出量" ), &q_ch4 );
        if( gas_type != GT_TUNNEL )
        {
            linkDoubleData( _T( "瓦斯涌出不均衡通风系数" ), &k_ch4 );
        }
        if( gas_type == GT_WS )
        {
            linkDoubleData( _T( "专排瓦斯巷瓦斯涌出量" ), &q_ch4_tail );
        }
    }

public:
    GAS_TYPE gas_type;
    double q_ch4;                 // 平均绝对瓦斯涌出量
    double k_ch4;                 // 瓦斯涌出不均衡通风系数
    double q_ch4_tail;            // 专排瓦斯巷瓦斯涌出量
};

IMPLEMENT_DYNAMIC( GasDlg, CDialog )

GasDlg::GasDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( GasDlg::IDD, pParent )
    , q_ch4( 0 )
    , k_ch4( 0 )
    , q_ch4_tail( 0 )
{

}

GasDlg::~GasDlg()
{
}

void GasDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_GAS_Q_CH4_EDIT, q_ch4 );
    DDX_Text( pDX, IDC_GAS_K_CH4_EDIT, k_ch4 );
    DDX_Text( pDX, IDC_GAS_Q_CH4_TAIL_EDIT, q_ch4_tail );
}

BEGIN_MESSAGE_MAP( GasDlg, CDialog )
    ON_BN_CLICKED( IDOK, &GasDlg::OnBnClickedOk )
END_MESSAGE_MAP()


BOOL GasDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if( gas_type != GT_WS )
    {
        GetDlgItem( IDC_GAS_Q_CH4_TAIL_EDIT )->EnableWindow( FALSE );
    }
    readPropertyData();

    return TRUE;
}

void GasDlg::OnBnClickedOk()
{
    writePropertyData();

    OnOK();
}

void GasDlg::readPropertyData()
{
    GasDataLink dl;
    dl.gas_type = gas_type;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    q_ch4 = dl.q_ch4;
    if( gas_type != GT_TUNNEL )
    {
        k_ch4 = dl.k_ch4;
    }
    else
    {
        k_ch4 = 0;
        GetDlgItem( IDC_GAS_K_CH4_EDIT )->EnableWindow( FALSE );
    }

    if( gas_type == GT_WS )
    {
        q_ch4_tail = dl.q_ch4_tail;
    }
    else
    {
        q_ch4_tail = 0;
        GetDlgItem( IDC_GAS_Q_CH4_TAIL_EDIT )->EnableWindow( FALSE );
    }

    UpdateData( FALSE );
}

void GasDlg::writePropertyData()
{
    UpdateData( TRUE );

    GasDataLink dl;
    dl.gas_type = gas_type;
    dl.q_ch4 = q_ch4;
    if( gas_type != GT_TUNNEL )
    {
        dl.k_ch4 = k_ch4;
    }
    if( gas_type == GT_WS )
    {
        dl.q_ch4_tail = q_ch4_tail;
    }

    dl.setDataSource( m_objId );
    dl.updateData( true );
}