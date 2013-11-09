#include "stdafx.h"
#include "ChargeDlg.h"

#include "../MineGE/HelperClass.h"
#include "../MineGE/DataLink.h"

class ChargeDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "ÇâÆøÁ¿" ), &m_h2 );
    }

public:
    double m_h2;
};

IMPLEMENT_DYNAMIC( ChargeDlg, CDialog )

ChargeDlg::ChargeDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( ChargeDlg::IDD, pParent )
{

}

ChargeDlg::~ChargeDlg()
{
}

void ChargeDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_CHARGE_H2, m_h2 );
}

BEGIN_MESSAGE_MAP( ChargeDlg, CDialog )
    ON_BN_CLICKED( IDOK, &ChargeDlg::OnBnClickedOk )
END_MESSAGE_MAP()

void ChargeDlg::readPropertyData()
{
    ChargeDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_h2 = dl.m_h2;

    UpdateData( FALSE );
}

void ChargeDlg::writePropertyData()
{
    UpdateData( TRUE );

    ChargeDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_h2 = m_h2;
    dl.updateData( true );
}

void ChargeDlg::OnBnClickedOk()
{
    writePropertyData();

    OnOK();
}

BOOL ChargeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    readPropertyData();

    return TRUE;
}
