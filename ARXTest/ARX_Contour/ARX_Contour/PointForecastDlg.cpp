#include "stdafx.h"
#include "PointForecastDlg.h"

#include "ARX_Gas.h"
#include "ContourHelper.h"
#include "CADDrawHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

static void ClearList( CListCtrl& m_list )
{
    //����ɾ��������
    m_list.DeleteAllItems();
    //�õ�����
    int n = m_list.GetHeaderCtrl()->GetItemCount();
    //ɾ��������
    for( int i = 0; i < n; i++ )
    {
        m_list.DeleteColumn( 0 );
    }
}

static void BuildHeadColumns( CListCtrl& m_list, const AcStringArray fields )
{
    int len = fields.length();

    CRect rect;
    m_list.GetClientRect( &rect );

    int n = len + 3;
    if( n > 10 ) n = 10;
    double dw = rect.Width() / n;

    m_list.InsertColumn( 0, _T( "���" ), LVCFMT_LEFT, dw );
    m_list.InsertColumn( 1, _T( "x����" ), LVCFMT_LEFT, dw );
    m_list.InsertColumn( 2, _T( "y����" ), LVCFMT_LEFT, dw );
    for( int i = 0; i < len; i++ )
    {
        m_list.InsertColumn( i + 3, fields[i].kACharPtr(), LVCFMT_LEFT, dw );
    }
}

static void FillDatas( CListCtrl& m_list, const AcStringArray fields, const AcDbObjectIdArray& objIds )
{
    if( objIds.isEmpty() ) return;

    AcGePoint3dArray pts;
    GetInsertPoints( objIds, pts );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        m_list.InsertItem( i, _T( "xx" ) );

        CString num;
        num.Format( _T( "%d" ), i + 1 );
        m_list.SetItemText( i, 0, num );

        //ItemData* pData = new ItemData();
        //pData->iItem = i;
        //pData->objId = objIds[i];
        //m_list.SetItemData(i, (LPARAM)pData); // ��������

        // д��xֵ
        CString x;
        x.Format( _T( "%.3f" ), pts[i].x );
        m_list.SetItemText( i, 1, x );

        // д��yֵ
        CString y;
        y.Format( _T( "%.3f" ), pts[i].y );
        m_list.SetItemText( i, 2, y );

        int n = fields.length();
        for( int j = 0; j < n; j++ )
        {
            //m_list.SetItemData(i, i);
            CString value;
            DataHelper::GetPropertyData( objIds[i], fields[j].kACharPtr(), value );
            m_list.SetItemText( i, j + 3, value );
        }
    }
}

IMPLEMENT_DYNAMIC( PointForecastDlg, CAcUiDialog )

PointForecastDlg::PointForecastDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( PointForecastDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_gas( 0 )
{

}

PointForecastDlg::~PointForecastDlg()
{
}

void PointForecastDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_GCP_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_GCP_X_POS, m_xPos );
    DDX_Text( pDX, IDC_GCP_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_GCP_GAS, m_gas );
    DDX_Control( pDX, IDC_DATA_LIST, m_dataList );
}

BEGIN_MESSAGE_MAP( PointForecastDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &PointForecastDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_GCP_PT_BTN, &PointForecastDlg::OnBnClickedGcpPtBtn )
    ON_BN_CLICKED( IDC_GEN_CAD_ENTITY_BTN, &PointForecastDlg::OnBnClickedGenCadEntityBtn )
END_MESSAGE_MAP()

BOOL PointForecastDlg::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();

    m_pickBtn.AutoLoad();

    fillDataList();

    UpdateData( FALSE );

    return TRUE;
}

void PointForecastDlg::OnBnClickedOk()
{
    if( !UpdateData( TRUE ) ) return;

    // ����
    AcGePoint3d pt( m_xPos, m_yPos, 0 );

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

        // ����Ԥ��
        m_gas = ContourHelper::PointCacl( new_datas, pt );
    }
    UpdateData( FALSE );
}

void PointForecastDlg::OnBnClickedGcpPtBtn()
{
    BeginEditorCommand();
    AcGePoint3d pt;
    ArxUtilHelper::PromptPt( _T( "\n��ѡ����������: " ), pt );
    CompleteEditorCommand();

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;
    UpdateData( FALSE );
}

void PointForecastDlg::fillDataList()
{
    ClearList( m_dataList );

    CString type = GetGasTypeName( GCT_CONTENT );
    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );

    BuildHeadColumns( m_dataList, fields );

    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( type, objIds );

    FillDatas( m_dataList, fields, objIds );
}

void PointForecastDlg::OnBnClickedGenCadEntityBtn()
{
    AcGePoint3d pt( m_xPos, m_yPos, 0 );
    DrawEllipse( pt, 40, 40 );

    CString text;
    text.Format( _T( "Ԥ����˹����\n%.4f" ), m_gas );
    DrawMText( pt, 0, text, 10 );

    OnOK();
}
