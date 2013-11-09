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
        errorMsg = _T( "\n�û�ȡ������ѡ�����" );
        return false;
    }

    if( pts.length() < 3 )
    {
        errorMsg = _T( "ѡ������Ķ���ε��������3��" );
        return false;
    }

    //acutPrintf(_T("\nѰ��͹��..."));

    //Graham_ConvexHull_3D(pts, polygon); // ʹ��Graham�㷨
    if( !Melkman_ConvexHull_3D( pts, polygon ) ) // ʹ��Melkman�㷨
    {
        errorMsg = _T( "\nѰ�Ҷ����͹��ʧ��" );
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


// RegionForecastDlg ��Ϣ�������
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
        MessageBox( _T( "��ѡ����������" ) );
    }
    else
    {
        // ԭʼ����
        AcGePoint3dArray datas;
        GetGasDatas( GCT_CONTENT, datas );

        if( datas.isEmpty() )
        {
            MessageBox( _T( "û�����ݿ�����������Ԥ�����" ) );
            m_gas = 0;
        }
        else
        {
            // ��ȡ�߽���������
            AcGePoint3dArray bounds;
            GetBoundaryPoints( bounds );
            if( bounds.isEmpty() )
            {
                MessageBox( _T( "�����һ���պϵľ���߽�" ) );
                return;
            }

            // �ϲ��߽�����
            AcGePoint3dArray new_datas;
            ContourHelper::PreprocessPointSet( bounds, datas, new_datas );

            // ʹ��AcDbExtent��ȡ��С�߽��
            AcDbExtents ext;
            int len = m_polygon.length();
            for( int i = 0; i < len; i++ ) ext.addPoint( m_polygon[i] );

            // ����Ԥ��
            double z = ContourHelper::GridCacl( new_datas, ext.minPoint(), ext.maxPoint(), 10 );

            // ����������˹����
            m_gas = z * m_area * m_thickness;
        }
        UpdateData( FALSE );
    }
}

void RegionForecastDlg::OnBnClickedRegionCadEntity()
{
    if( m_polygon.isEmpty() )
    {
        MessageBox( _T( "��ѡ����������" ) );
        return;
    }

    DrawPLines( m_polygon, true );

    // ������������
    AcGePoint3d pt = PolygonCentroid( m_polygon );

    CString text;
    text.Format( _T( "Ԥ��������˹��\n�������: %.4f\nú���ȣ�%.4f\nú���أ�%.4f\n������˹��: %.4f" ), m_area, m_thickness, m_weight, m_gas );
    DrawMText( pt, 0, text, 10 );

    OnOK();
}
