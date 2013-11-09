#include "stdafx.h"

#include "VNG_ParamDlg.h"
#include "VNG_ParamHelper.h"

IMPLEMENT_DYNAMIC( VNG_ParamDlg, CDialog )

VNG_ParamDlg::VNG_ParamDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( VNG_ParamDlg::IDD, pParent )
    , m_nodeSep( 0 )
    , m_rankSep( 0 )
    , m_arrowWidth( 0 )
    , m_arrowLength( 0 )
    , m_edgeTextHeight( 0 )
    , m_needEdge( FALSE )
    , m_graphRatio( 0 )
    , m_graphWidth( 0 )
    , m_graphHeight( 0 )
    , m_nodeWidth( 0 )
    , m_nodeHeight( 0 )
    , m_nodeTextHeight( 0 )
    , m_useDefWH( FALSE )
{

}

VNG_ParamDlg::~VNG_ParamDlg()
{
}

void VNG_ParamDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_VNG_GEN_NODESEP, m_nodeSep );
    DDX_Text( pDX, IDC_VNG_GEN_RANKSEP, m_rankSep );
    DDV_MinMaxDouble( pDX, m_nodeSep, 0.02, DBL_MAX );
    DDV_MinMaxDouble( pDX, m_rankSep, 0.02, DBL_MAX );

    DDX_Text( pDX, IDC_VNG_GEN_RATIO, m_graphRatio );
    DDX_Text( pDX, IDC_VNG_GEN_WIDTH, m_graphWidth );
    DDX_Text( pDX, IDC_VNG_GEN_HEIGHT, m_graphHeight );

    DDX_Text( pDX, IDC_VNG_GEN_ARROW_WIDTH, m_arrowWidth );
    DDX_Text( pDX, IDC_VNG_GEN_ARROW_LENGTH, m_arrowLength );
    DDX_Text( pDX, IDC_VNG_GEN_NODE_TEXT_HEIGHT, m_nodeTextHeight );

    DDX_Text( pDX, IDC_VNG_GEN_EDGE_TEXT_HEIGHT, m_edgeTextHeight );
    DDX_Check( pDX, IDC_VNG_GEN_NEED_EDGE, m_needEdge );
    DDX_Text( pDX, IDC_VNG_GEN_NODE_WIDTH, m_nodeWidth );
    DDX_Text( pDX, IDC_VNG_GEN_NODE_HEIGHT, m_nodeHeight );
    DDX_Check( pDX, IDC_VNG_GEN_USE_DEF_WH, m_useDefWH );
}


BEGIN_MESSAGE_MAP( VNG_ParamDlg, CDialog )
    ON_BN_CLICKED( IDOK, &VNG_ParamDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_VNG_GEN_USE_DEF_WH, &VNG_ParamDlg::OnBnClickedVngGenUseDefWh )
END_MESSAGE_MAP()


// VNG_ParamDlg 消息处理程序
BOOL VNG_ParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    bool useDefWH, needEdge;
    VNG_ParamHelper::ReadParams( m_nodeSep, m_rankSep,
                                 m_graphRatio, m_graphWidth, m_graphHeight, useDefWH,
                                 m_nodeWidth, m_nodeHeight, m_nodeTextHeight,
                                 m_arrowWidth, m_arrowLength, m_edgeTextHeight, needEdge );
    m_useDefWH = ( useDefWH ? 1 : 0 );
    m_needEdge = ( needEdge ? 1 : 0 );

    UpdateData( FALSE );

    togWHControls( !m_useDefWH );

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void VNG_ParamDlg::OnBnClickedOk()
{
    UpdateData( TRUE );

    VNG_ParamHelper::WriteParams( m_nodeSep, m_rankSep,
                                  m_graphRatio, m_graphWidth, m_graphHeight, ( m_useDefWH != 0 ),
                                  m_nodeWidth, m_nodeHeight, m_nodeTextHeight,
                                  m_arrowWidth, m_arrowLength, m_edgeTextHeight, ( m_needEdge != 0 ) );

    OnOK();
}

void VNG_ParamDlg::OnBnClickedVngGenUseDefWh()
{
    UpdateData( TRUE );

    togWHControls( !m_useDefWH );
}

void VNG_ParamDlg::togWHControls( BOOL flag )
{
    GetDlgItem( IDC_VNG_GEN_RATIO )->EnableWindow( flag );
    GetDlgItem( IDC_VNG_GEN_WIDTH )->EnableWindow( flag );
    GetDlgItem( IDC_VNG_GEN_HEIGHT )->EnableWindow( flag );
}