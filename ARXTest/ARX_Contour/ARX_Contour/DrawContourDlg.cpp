#include "stdafx.h"
#include "DrawContourDlg.h"

#include "ARX_Gas.h"
#include "ContourHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include <algorithm>
#include <iterator>

#define CSC_EPLISON 0.001

static bool IsFloatEqual( double a, double b )
{
    return abs( a - b ) < CSC_EPLISON;
}

static bool IsFloatLower( double a, double b )
{
    return ( a - b ) < CSC_EPLISON;
}

static void DrawContour( const CString& layer, AcGePoint3dArray& bounds, const AcGePoint3dArray& datas, double minz, double maxZ, double dz )
{
    if( datas.isEmpty() ) return;

    // 删除图层上的所有等值线图元
    AcDbObjectIdArray objIds;
    ContourHelper::GetContourGEOnLayer( layer, _T( "ContourGE" ), objIds );
    ArxEntityHelper::EraseObjects2( objIds, true );

    // 临时切换图层，如果图层不存在，则新建图层
    LayerSwitch ls( layer, true );

    // 绘制等值线
    ContourHelper::DrawCountourByDeltaZ( bounds, datas, minz, maxZ, dz );
}

IMPLEMENT_DYNAMIC( DrawContourDlg, CDialog )

DrawContourDlg::DrawContourDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( DrawContourDlg::IDD, pParent )
    , m_data_nCount( 0 )
    , m_data_maxZ( 0 )
    , m_data_minZ( 0 )
    , m_dz( 0 )
    , m_maxZ( 0 )
    , m_minZ( 0 )
{

}

DrawContourDlg::~DrawContourDlg()
{
}

void DrawContourDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_CONTOUR_TYPE_COMBO, m_contourTypeCombo );
    DDX_Text( pDX, IDC_CSC_MAX_VALUE, m_data_maxZ );
    DDX_Text( pDX, IDC_CSC_MIN_VALUE, m_data_minZ );
    DDX_Text( pDX, IDC_CSC_DATA_COUNT, m_data_nCount );

    DDX_Control( pDX, IDC_CSC_LAYER_LIST, m_layerList );
    DDX_Text( pDX, IDC_CSC_MAX_VALUE2, m_maxZ );
    DDX_Text( pDX, IDC_CSC_MIN_VALUE2, m_minZ );
    DDX_Text( pDX, IDC_CSC_DELTA_VALUE, m_dz );
}

BEGIN_MESSAGE_MAP( DrawContourDlg, CDialog )
    ON_BN_CLICKED( IDOK, &DrawContourDlg::OnBnClickedOk )
    ON_CBN_SELCHANGE( IDC_CONTOUR_TYPE_COMBO, &DrawContourDlg::OnCbnSelchangeContourTypeCombo )
END_MESSAGE_MAP()

BOOL DrawContourDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_contourTypeCombo.AddString( _T( "瓦斯涌出量等值线" ) );
    m_contourTypeCombo.AddString( _T( "瓦斯含量等值线" ) );
    m_contourTypeCombo.AddString( _T( "瓦斯压力等值线" ) );
    //m_contourTypeCombo.SetCurSel(0);

    //GetDlgItem(IDC_GFP_COLOR_BTN)->EnableWindow(FALSE);

    // 填充图层列表
    FillLayerList();

    //updateContourData(m_contourTypeCombo.GetCurSel());
    //UpdateData(FALSE);

    return TRUE;
}

bool DrawContourDlg::validateContourData()
{
    if( IsFloatLower( m_maxZ, m_minZ ) )
    {
        MessageBox( _T( "【等值线参数】最大值必须大于最小值" ) );
        return false;
    }

    if( IsFloatLower( m_dz, 0 ) )
    {
        MessageBox( _T( "【等值线参数】间距必须大于0!!!" ) );
        return false;
    }

    if( IsFloatLower( m_maxZ - m_minZ, m_dz ) )
    {
        MessageBox( _T( "【等值线参数】间距不能超过(最大值-最小值)" ) );
        return false;
    }
    return true;
}

void DrawContourDlg::OnBnClickedOk()
{
    if( !UpdateData( TRUE ) ) return;

    if( CB_ERR == m_contourTypeCombo.GetCurSel() )
    {
        MessageBox( _T( "请选择等值线类型" ) );
        return;
    }

    if( m_data_nCount == 0 )
    {
        MessageBox( _T( "请添加数据" ) );
        return;
    }

    // 读取图层
    CString layer;
    m_layerList.GetWindowText( layer );
    if( layer.GetLength() == 0 )
    {
        MessageBox( _T( "请选择或输入一个图层名称!!!" ) );
        return ;
    }

    if( !validateContourData() ) return;

    // 获取边界坐标数据
    AcGePoint3dArray bounds;
    GetBoundaryPoints( bounds );
    if( bounds.isEmpty() )
    {
        MessageBox( _T( "请添加一个闭合的井田边界" ) );
        return;
    }

    // 原始数据
    AcGePoint3dArray datas;
    GetGasDatas( ( GasContourType )m_contourTypeCombo.GetCurSel(), datas );
    if( datas.isEmpty() )
    {
        MessageBox( _T( "没有数据, 请在菜单中使用绘图命令增加数据" ) );
        return;
    }

    // 绘制等值线
    DrawContour( layer, bounds, datas, m_minZ, m_maxZ, m_dz );

    OnOK();
}

static bool IsLayerHasContour( const CString& layer )
{
    AcDbObjectIdArray objIds;
    ContourHelper::GetContourGEOnLayer( layer, _T( "ContourGE" ), objIds );
    return !objIds.isEmpty();
}

// 对图层进行排序，将包含等值线的图层排列在前面
static void SortLayers( AcStringArray& layers )
{
    AcStringArray layers1, layers2;
    int n = layers.length();
    for( int i = 0; i < n; i++ )
    {
        CString layer = layers[i].kACharPtr();
        if( IsLayerHasContour( layer ) )
        {
            layers1.append( layer );
        }
        else
        {
            layers2.append( layer );
        }
    }

    layers.removeAll();
    layers.append( layers1 );
    layers.append( layers2 );
}

void DrawContourDlg::FillLayerList()
{
    m_layerList.ResetContent();

    // 读取所有图层列表
    AcStringArray layers;
    LayerHelper::GetAllLayers( layers );

    // 对图层进行排序，将有等值线的图层排列在前面
    //SortLayers(layers);

    // 填充列表
    int n = layers.length();
    for( int i = 0; i < n; i++ )
    {
        m_layerList.AddString( layers[i].kACharPtr() );
    }

    if( m_layerList.GetCount() != 0 )
    {
        m_layerList.SetCurSel( 0 );
    }
}

void DrawContourDlg::updateContourData( int gct )
{
    AcGePoint3dArray datas;
    GetGasDatas( ( GasContourType )gct, datas );

    m_data_nCount = datas.length();

    if( m_data_nCount > 0 )
    {
        ContourHelper::GetMinMaxZValue( datas, m_data_minZ, m_data_maxZ );
    }
    else
    {
        m_data_maxZ = 0;
        m_data_minZ = 0;
    }
}

void DrawContourDlg::OnCbnSelchangeContourTypeCombo()
{
    UpdateData( TRUE );

    int pos = m_contourTypeCombo.GetCurSel();
    updateContourData( pos );

    // 瓦斯涌出量等值线需要设置颜色
    //GetDlgItem(IDC_GFP_COLOR_BTN)->EnableWindow((pos==1)?TRUE:FALSE);

    UpdateData( FALSE );
}
