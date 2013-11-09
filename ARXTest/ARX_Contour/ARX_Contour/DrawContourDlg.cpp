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

    // ɾ��ͼ���ϵ����е�ֵ��ͼԪ
    AcDbObjectIdArray objIds;
    ContourHelper::GetContourGEOnLayer( layer, _T( "ContourGE" ), objIds );
    ArxEntityHelper::EraseObjects2( objIds, true );

    // ��ʱ�л�ͼ�㣬���ͼ�㲻���ڣ����½�ͼ��
    LayerSwitch ls( layer, true );

    // ���Ƶ�ֵ��
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

    m_contourTypeCombo.AddString( _T( "��˹ӿ������ֵ��" ) );
    m_contourTypeCombo.AddString( _T( "��˹������ֵ��" ) );
    m_contourTypeCombo.AddString( _T( "��˹ѹ����ֵ��" ) );
    //m_contourTypeCombo.SetCurSel(0);

    //GetDlgItem(IDC_GFP_COLOR_BTN)->EnableWindow(FALSE);

    // ���ͼ���б�
    FillLayerList();

    //updateContourData(m_contourTypeCombo.GetCurSel());
    //UpdateData(FALSE);

    return TRUE;
}

bool DrawContourDlg::validateContourData()
{
    if( IsFloatLower( m_maxZ, m_minZ ) )
    {
        MessageBox( _T( "����ֵ�߲��������ֵ���������Сֵ" ) );
        return false;
    }

    if( IsFloatLower( m_dz, 0 ) )
    {
        MessageBox( _T( "����ֵ�߲��������������0!!!" ) );
        return false;
    }

    if( IsFloatLower( m_maxZ - m_minZ, m_dz ) )
    {
        MessageBox( _T( "����ֵ�߲�������಻�ܳ���(���ֵ-��Сֵ)" ) );
        return false;
    }
    return true;
}

void DrawContourDlg::OnBnClickedOk()
{
    if( !UpdateData( TRUE ) ) return;

    if( CB_ERR == m_contourTypeCombo.GetCurSel() )
    {
        MessageBox( _T( "��ѡ���ֵ������" ) );
        return;
    }

    if( m_data_nCount == 0 )
    {
        MessageBox( _T( "���������" ) );
        return;
    }

    // ��ȡͼ��
    CString layer;
    m_layerList.GetWindowText( layer );
    if( layer.GetLength() == 0 )
    {
        MessageBox( _T( "��ѡ�������һ��ͼ������!!!" ) );
        return ;
    }

    if( !validateContourData() ) return;

    // ��ȡ�߽���������
    AcGePoint3dArray bounds;
    GetBoundaryPoints( bounds );
    if( bounds.isEmpty() )
    {
        MessageBox( _T( "�����һ���պϵľ���߽�" ) );
        return;
    }

    // ԭʼ����
    AcGePoint3dArray datas;
    GetGasDatas( ( GasContourType )m_contourTypeCombo.GetCurSel(), datas );
    if( datas.isEmpty() )
    {
        MessageBox( _T( "û������, ���ڲ˵���ʹ�û�ͼ������������" ) );
        return;
    }

    // ���Ƶ�ֵ��
    DrawContour( layer, bounds, datas, m_minZ, m_maxZ, m_dz );

    OnOK();
}

static bool IsLayerHasContour( const CString& layer )
{
    AcDbObjectIdArray objIds;
    ContourHelper::GetContourGEOnLayer( layer, _T( "ContourGE" ), objIds );
    return !objIds.isEmpty();
}

// ��ͼ��������򣬽�������ֵ�ߵ�ͼ��������ǰ��
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

    // ��ȡ����ͼ���б�
    AcStringArray layers;
    LayerHelper::GetAllLayers( layers );

    // ��ͼ��������򣬽��е�ֵ�ߵ�ͼ��������ǰ��
    //SortLayers(layers);

    // ����б�
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

    // ��˹ӿ������ֵ����Ҫ������ɫ
    //GetDlgItem(IDC_GFP_COLOR_BTN)->EnableWindow((pos==1)?TRUE:FALSE);

    UpdateData( FALSE );
}
