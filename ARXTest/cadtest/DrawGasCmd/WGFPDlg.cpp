#include "stdafx.h"
#include "WGFPDlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/GFP.h"

class WGFPDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "������˹ӿ����" ), &m_aq );
        linkDoubleData( _T( "�����˹ӿ����" ), &m_rq );
        linkDoubleData( _T( "�������ղ���" ), &m_t );
        linkStringData( _T( "�ز�����" ), &m_dt );
    }

public:
    double m_aq;
    double m_rq;
    double m_t;
    CString m_dt;
};

static WGFP* CreateWGFP( const AcGePoint3d& pt )
{
    return new WGFP( pt );
}

IMPLEMENT_DYNAMIC( WGFPDlg, CAcUiDialog )

WGFPDlg::WGFPDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( WGFPDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_aq( 8.4 )
    , m_rq( 3.06 )
    , m_t( 3956 )
    , m_dt( _T( "03.03" ) )
{

}

WGFPDlg::~WGFPDlg()
{
}

void WGFPDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_WGFP_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_WGFP_X_POS, m_xPos );
    DDX_Text( pDX, IDC_WGFP_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_WGFP_Z_POS, m_zPos );

    DDX_Text( pDX, IDC_WGFP_AQ, m_aq );
    DDX_Text( pDX, IDC_WGFP_RQ, m_rq );
    DDX_Text( pDX, IDC_WGFP_T, m_t );
    DDX_Text( pDX, IDC_WGFP_DATE, m_dt );
}


BEGIN_MESSAGE_MAP( WGFPDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &WGFPDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_WGFP_PT_BTN, &WGFPDlg::OnBnClickedGopPtBtn )
END_MESSAGE_MAP()


BOOL WGFPDlg::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();

    m_pickBtn.AutoLoad();

    if( !m_objId.isNull() )
    {
        readPropertyData();
        UpdateData( FALSE );

        GetDlgItem( IDOK )->SetWindowText( _T( "�޸�" ) );
    }
    else
    {
        GetDlgItem( IDOK )->SetWindowText( _T( "���" ) );
    }

    return TRUE;
}

void WGFPDlg::OnBnClickedOk()
{
    if( !UpdateData( TRUE ) ) return;

    if( m_objId.isNull() )
    {
        m_objId = createObject();
        if( m_objId.isNull() )
        {
            MessageBox( _T( "���ʧ��" ) );
            OnOK();
            return;
        }
    }

    writePropertyData();
    OnOK();
}

void WGFPDlg::OnBnClickedGopPtBtn()
{
    BeginEditorCommand();
    AcGePoint3d pt;
    ArxUtilHelper::PromptPt( _T( "��ѡ����������: " ), pt );
    CompleteEditorCommand();

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;
    UpdateData( FALSE );
}

void WGFPDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    WGFPDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_aq = dl.m_aq;
    m_rq = dl.m_rq;
    m_t = dl.m_t;
    m_dt = dl.m_dt;
}

void WGFPDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    WGFPDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_aq = m_aq;
    dl.m_rq = m_rq;
    dl.m_t = m_t;
    dl.m_dt = m_dt;

    dl.updateData( true );
}

AcDbObjectId WGFPDlg::createObject()
{
    AcDbObjectId objId;

    WGFP* pWGFP = CreateWGFP( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pWGFP ) )
    {
        objId = pWGFP->objectId();
        pWGFP->close();
    }
    else
    {
        delete pWGFP;
    }
    return objId;
}

