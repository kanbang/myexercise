#include "stdafx.h"
#include "RegionForecastDlg.h"

#include "ARX_Gas.h"
#include "ARX_GeoAlgo.h"
#include "PolyLineJig.h"
#include "ContourHelper.h"
#include "CADDrawHelper.h"

static bool GetPolyon( AcGePoint3dArray& polygon, CString& errorMsg )
{
    AcGePoint3dArray pts;
    if( !GetPolygonByCmd( pts ) )
    {
        errorMsg = _T( "\n用户取消区域选择过程" );
        return false;
    }

    if( pts.length() < 3 )
    {
        errorMsg = _T( "选择区域的多边形点个数少于3个" );
        return false;
    }

    //acutPrintf(_T("\n寻找凸包..."));

    //Graham_ConvexHull_3D(pts, polygon); // 使用Graham算法
    if( !Melkman_ConvexHull_3D( pts, polygon ) ) // 使用Melkman算法
    {
        errorMsg = _T( "\n寻找多边形凸包失败" );
        return false;
    }
    return true;
}

IMPLEMENT_DYNAMIC( RegionForecastDlg, CAcUiDialog )

RegionForecastDlg::RegionForecastDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( RegionForecastDlg::IDD, pParent )
    , m_thickness( 3 )
    , m_weight( 1.3 )
    , m_area( 0 )
    , m_gas( 0 )
    , m_count( 0 )
{

}

RegionForecastDlg::~RegionForecastDlg()
{
}

void RegionForecastDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_GCP_LEVEL, m_thickness );
    DDX_Text( pDX, IDC_GCP_HEIGHT, m_weight );
    DDX_Text( pDX, IDC_REGION_AREA, m_area );
    DDX_Text( pDX, IDC_REGION_GAS, m_gas );
    DDX_Text( pDX, IDC_POLYGON_VERTEX_COUNT, m_count );
}


BEGIN_MESSAGE_MAP( RegionForecastDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &RegionForecastDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_REGION_CAD_ENTITY, &RegionForecastDlg::OnBnClickedRegionCadEntity )
    ON_BN_CLICKED( IDC_REGION_SELECT_BTN, &RegionForecastDlg::OnBnClickedRegionSelectBtn )
END_MESSAGE_MAP()


// RegionForecastDlg 消息处理程序
void RegionForecastDlg::OnBnClickedRegionSelectBtn()
{
    BeginEditorCommand();

    AcGePoint3dArray polygon;
    CString msg;
    bool ret = GetPolyon( polygon, msg );

    //CancelEditorCommand();
    CompleteEditorCommand();

    if( !ret )
    {
        MessageBox( msg );
    }
    else
    {
        m_polygon.removeAll();
        m_polygon.append( polygon );

        m_count = m_polygon.length();
        m_area = abs( PolygonArea( m_polygon ) );
    }

    UpdateData( FALSE );
}

void RegionForecastDlg::OnBnClickedOk()
{
    UpdateData( TRUE );

    if( m_polygon.isEmpty() )
    {
        MessageBox( _T( "请选择区域多边形" ) );
    }
    else
    {
        // 原始数据
        AcGePoint3dArray datas;
        GetGasDatas( GCT_CONTENT, datas );

        if( datas.isEmpty() )
        {
            MessageBox( _T( "没有数据可以用来进行预测分析" ) );
            m_gas = 0;
        }
        else
        {
            // 获取边界坐标数据
            AcGePoint3dArray bounds;
            GetBoundaryPoints( bounds );
            if( bounds.isEmpty() )
            {
                MessageBox( _T( "请添加一个闭合的井田边界" ) );
                return;
            }

            // 合并边界数据
            AcGePoint3dArray new_datas;
            ContourHelper::PreprocessPointSet( bounds, datas, new_datas );

            // 使用AcDbExtent获取最小边界盒
            AcDbExtents ext;
            int len = m_polygon.length();
            for( int i = 0; i < len; i++ ) ext.addPoint( m_polygon[i] );

            // 区域预测
            double z = ContourHelper::GridCacl( new_datas, ext.minPoint(), ext.maxPoint(), 10 );

            // 计算区域瓦斯含量
            m_gas = z * m_area * m_thickness;
        }
        UpdateData( FALSE );
    }
}

void RegionForecastDlg::OnBnClickedRegionCadEntity()
{
    if( m_polygon.isEmpty() )
    {
        MessageBox( _T( "请选择区域多边形" ) );
        return;
    }

    DrawPLines( m_polygon, true );

    // 计算多边形形心
    AcGePoint3d pt = PolygonCentroid( m_polygon );

    CString text;
    text.Format( _T( "预测区域瓦斯量\n区域面积: %.4f\n煤层厚度：%.4f\n煤容重：%.4f\n区域瓦斯量: %.4f" ), m_area, m_thickness, m_weight, m_gas );
    DrawMText( pt, 0, text, 10 );

    OnOK();
}
