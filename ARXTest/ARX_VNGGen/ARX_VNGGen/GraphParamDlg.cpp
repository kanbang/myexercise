#include "stdafx.h"

#include "GraphParamDlg.h"

IMPLEMENT_DYNAMIC( GraphParamDlg, CDialog )

GraphParamDlg::GraphParamDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( GraphParamDlg::IDD, pParent )
    , m_nodeSep( 0 )
    , m_rankSep( 0 )
    , m_graphRatio( 0 )
    , m_graphWidth( 0 )
    , m_graphHeight( 0 )
    , m_useDefWH( FALSE )
{

}

GraphParamDlg::~GraphParamDlg()
{
}

void GraphParamDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_VNG_GEN_NODESEP, m_nodeSep );
    DDX_Text( pDX, IDC_VNG_GEN_RANKSEP, m_rankSep );
    DDV_MinMaxDouble( pDX, m_nodeSep, 0.02, DBL_MAX );
    DDV_MinMaxDouble( pDX, m_rankSep, 0.02, DBL_MAX );

    DDX_Text( pDX, IDC_VNG_GEN_RATIO, m_graphRatio );
    DDX_Text( pDX, IDC_VNG_GEN_WIDTH, m_graphWidth );
    DDX_Text( pDX, IDC_VNG_GEN_HEIGHT, m_graphHeight );
    DDX_Check( pDX, IDC_VNG_GEN_USE_DEF_WH, m_useDefWH );
}


BEGIN_MESSAGE_MAP( GraphParamDlg, CDialog )
    ON_BN_CLICKED( IDOK, &GraphParamDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_VNG_GEN_USE_DEF_WH, &GraphParamDlg::OnBnClickedVngGenUseDefWh )
END_MESSAGE_MAP()


// GraphParamDlg 消息处理程序
BOOL GraphParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_nodeSep = gp.m_nodeSep;
    m_rankSep = gp.m_rankSep;
    m_graphRatio = gp.m_graphRatio;
    m_graphWidth = gp.m_graphWidth;
    m_graphHeight = gp.m_graphHeight;
    m_useDefWH = ( int )gp.m_useDefWH;

    UpdateData( FALSE );

    togWHControls( !m_useDefWH );

    return TRUE;
}

void GraphParamDlg::OnBnClickedOk()
{
    if( UpdateData( TRUE ) )
    {
        gp.m_nodeSep = m_nodeSep;
        gp.m_rankSep = m_rankSep;
        gp.m_graphRatio = m_graphRatio;
        gp.m_graphWidth = m_graphWidth;
        gp.m_graphHeight = m_graphHeight;
        gp.m_useDefWH = ( gp.m_useDefWH != 0 );

        OnOK();
    }
}

void GraphParamDlg::OnBnClickedVngGenUseDefWh()
{
    UpdateData( TRUE );

    togWHControls( !m_useDefWH );
}

void GraphParamDlg::togWHControls( BOOL flag )
{
    GetDlgItem( IDC_VNG_GEN_RATIO )->EnableWindow( flag );
    GetDlgItem( IDC_VNG_GEN_WIDTH )->EnableWindow( flag );
    GetDlgItem( IDC_VNG_GEN_HEIGHT )->EnableWindow( flag );
}