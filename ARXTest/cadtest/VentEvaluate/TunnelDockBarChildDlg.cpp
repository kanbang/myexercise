#include "StdAfx.h"
#include "TunnelDockBarChildDlg.h"

#include "../MineGE/ConstData.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "AFCDlg.h"

IMPLEMENT_DYNAMIC ( TunnelDockBarChildDlg, PropertyData_DockBarChildDlg )

BEGIN_MESSAGE_MAP( TunnelDockBarChildDlg, PropertyData_DockBarChildDlg )
    //{{AFX_MSG_MAP(TunnelDockBarChildDlg)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED( IDC_OT_AFC_BTN, &TunnelDockBarChildDlg::OnBnClickedAfcBtn )
    ON_BN_CLICKED( IDC_TUNNEL_TYPE_NEED_AIR, &TunnelDockBarChildDlg::OnBnClickedTunnelTypeNeedAir )
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
TunnelDockBarChildDlg::TunnelDockBarChildDlg ( CWnd* pParent, HINSTANCE hInstance )
    : PropertyData_DockBarChildDlg ( TunnelDockBarChildDlg::IDD, pParent, hInstance )
    , m_needAir( FALSE )
    , m_name( _T( "" ) )
{
    //{{AFX_DATA_INIT(TunnelDockBarChildDlg)
    //}}AFX_DATA_INIT
}

TunnelDockBarChildDlg::~TunnelDockBarChildDlg()
{
}

void TunnelDockBarChildDlg::DoDataExchange ( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange ( pDX ) ;
    //{{AFX_DATA_MAP(TunnelDockBarChildDlg)
    //}}AFX_DATA_MAP
    DDX_Check( pDX, IDC_TUNNEL_TYPE_NEED_AIR, m_needAir );
    DDX_Text( pDX, IDC_TUNNEL_NAME_EDIT, m_name );
    DDX_Text( pDX, IDC_TUNNEL_AREA_EDIT, m_area );
    DDX_Text( pDX, IDC_TUNNEL_VELOCITY_EDIT, m_velocity );
    DDX_Text( pDX, IDC_TUNNEL_Q_EDIT, m_q );
    DDX_Text( pDX, IDC_TUNNEL_TEMP_EDIT, m_temp );
    DDX_Control( pDX, IDC_TUNNEL_MATERIAL_TYPE, m_combo );
    DDX_Control( pDX, IDC_TUNNEL_FUNC_TYPE, m_ttCombo );
}

void TunnelDockBarChildDlg::readPropertyData()
{
    CString v1;
    DataHelper::GetPropertyData( m_objId, _T( "功能类型" ), v1 );
    int i = 0;
    ArxUtilHelper::StringToInt( v1, i );
    m_ttCombo.SetCurSel( i % 6 ); // 设置巷道用途
    int ttype = i % 6;

    CString v2;
    DataHelper::GetPropertyData( m_objId, _T( "是否单独配风" ), v2 );
    int i2 = 0;
    ArxUtilHelper::StringToInt( v2, i2 );
    m_needAir = i2 % 2;

    if( ttype == FT_ELECTRIC || ttype == FT_DIESEL )
    {
        m_needAir = TRUE; // 架线电机车巷和胶轮车巷强制单独配风
    }

    if( m_needAir )
    {
        GetDlgItem( IDC_OT_AFC_BTN )->EnableWindow( TRUE );
    }
    else
    {
        GetDlgItem( IDC_OT_AFC_BTN )->EnableWindow( FALSE );
    }

    // 读取巷道的其它参数
    DataHelper::GetPropertyData( m_objId, _T( "名称" ), m_name );
    DataHelper::GetPropertyData( m_objId, _T( "断面面积" ), m_area );
    DataHelper::GetPropertyData( m_objId, _T( "风速" ), m_velocity );
    DataHelper::GetPropertyData( m_objId, _T( "风量" ), m_q );
    DataHelper::GetPropertyData( m_objId, _T( "温度" ), m_temp );

    CString v3;
    DataHelper::GetPropertyData( m_objId, _T( "材质类型" ), v3 );
    int i3 = 0;
    ArxUtilHelper::StringToInt( v3, i3 );
    i3 = i3 % 3;
    m_combo.SetCurSel( i3 );

}

void TunnelDockBarChildDlg::writePropertyData()
{
    int ttype = m_ttCombo.GetCurSel();
    CString v1;
    v1.Format( _T( "%d" ), ttype );
    DataHelper::SetPropertyData( m_objId, _T( "功能类型" ), v1 );

    if( ttype == FT_ELECTRIC || ttype == FT_DIESEL )
    {
        m_needAir = TRUE; // 架线电机车巷和胶轮车巷强制单独配风
    }

    CString v2;
    v2.Format( _T( "%d" ), ( int )m_needAir );
    DataHelper::SetPropertyData( m_objId, _T( "是否单独配风" ), v2 );

    // 设置巷道的其它参数
    DataHelper::SetPropertyData( m_objId, _T( "名称" ), m_name );
    DataHelper::SetPropertyData( m_objId, _T( "断面面积" ), m_area );
    DataHelper::SetPropertyData( m_objId, _T( "风速" ), m_velocity );
    DataHelper::SetPropertyData( m_objId, _T( "风量" ), m_q );
    DataHelper::SetPropertyData( m_objId, _T( "温度" ), m_temp );

    CString v3;
    v3.Format( _T( "%d" ), m_combo.GetCurSel() );
    DataHelper::SetPropertyData( m_objId, _T( "材质类型" ), v3 );
}

BOOL TunnelDockBarChildDlg::OnInitDialog()
{
    PropertyData_DockBarChildDlg::OnInitDialog();

    m_combo.AddString( _T( "岩巷" ) );
    m_combo.AddString( _T( "煤巷" ) );
    m_combo.AddString( _T( "半煤岩巷" ) );

    // 对应TUNNEL_TYPE,参见../MineGE/ConstData.h
    m_ttCombo.AddString( _T( "通风人行巷道" ) );
    m_ttCombo.AddString( _T( "主要运输或进回大巷" ) );
    m_ttCombo.AddString( _T( "采区运输或进回大巷" ) );
    m_ttCombo.AddString( _T( "工作面运输机巷" ) );
    m_ttCombo.AddString( _T( "架线电机车巷" ) );
    m_ttCombo.AddString( _T( "胶轮车巷" ) );

    return TRUE;
}

void TunnelDockBarChildDlg::OnBnClickedAfcBtn()
{
    UpdateData( TRUE );

    CAcModuleResourceOverride myResources;

    acDocManager->lockDocument( curDoc() );

    AFCDlg dlg;
    dlg.m_objId = m_objId;
    dlg.afc_type = AT_OT;
    dlg.m_isDiesel = ( m_ttCombo.GetCurSel() == FT_DIESEL );
    dlg.DoModal();

    acDocManager->unlockDocument( curDoc() );
}

void TunnelDockBarChildDlg::OnBnClickedTunnelTypeNeedAir()
{
    UpdateData( TRUE );

    int ttype = m_ttCombo.GetCurSel();
    if( ttype == FT_ELECTRIC || ttype == FT_DIESEL )
    {
        m_needAir = TRUE;
    }
    UpdateData( FALSE );

    if( !m_needAir )
    {
        GetDlgItem( IDC_OT_AFC_BTN )->EnableWindow( FALSE );
    }
    else
    {
        GetDlgItem( IDC_OT_AFC_BTN )->EnableWindow( TRUE );
    }
}