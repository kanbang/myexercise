#include "stdafx.h"
#include "MineInfoDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

IMPLEMENT_DYNAMIC( MineInfoDlg, CDialog )

MineInfoDlg::MineInfoDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( MineInfoDlg::IDD, pParent )
    , m_designDate( COleDateTime::GetCurrentTime() )
    , m_workDate( COleDateTime::GetCurrentTime() )
    , m_name( _T( "" ) )
    , m_company( _T( "" ) )
    , m_quality( _T( "" ) )
    , m_totalQ( _T( "" ) )
{

}

MineInfoDlg::~MineInfoDlg()
{
}

void MineInfoDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_DateTimeCtrl( pDX, IDC_MINE_DESIGN_DATE, m_designDate );
    DDX_DateTimeCtrl( pDX, IDC_MINE_WORK_DATE, m_workDate );
    DDX_Text( pDX, IDC_MINE_NAME, m_name );
    DDX_Text( pDX, IDC_MINE_COMPANY, m_company );
    DDX_Text( pDX, IDC_MINE_QUALITY, m_quality );
    DDX_Control( pDX, IDC_MINE_GAS_LEVEL, m_gasLevel );
    DDX_Control( pDX, IDC_MINE_VENT, m_vent );
    DDX_Control( pDX, IDC_MINE_INFO_SLIDER1, m_slider1 );
    DDX_Control( pDX, IDC_MINE_INFO_SLIDER2, m_slider2 );
    DDX_Text( pDX, IDC_TOTAL_Q, m_totalQ );
}


BEGIN_MESSAGE_MAP( MineInfoDlg, CDialog )
    ON_BN_CLICKED( IDOK, &MineInfoDlg::OnBnClickedOk )
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


// MineInfoDlg ��Ϣ��������

static void GetTimeFromData( const AcDbObjectId& objId, const CString& field, COleDateTime& mt )
{
    CString value;
    DataHelper::GetPropertyData( objId, field, value );
    mt.ParseDateTime( value );
}

static void SetTimeToData( const AcDbObjectId& objId, const CString& field, const COleDateTime& mt )
{
    CString value = ( COleVariant )mt;
    DataHelper::SetPropertyData( objId, field, value );
}

BOOL MineInfoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    readPropertyData();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void MineInfoDlg::OnBnClickedOk()
{
    writePropertyData();

    OnOK();
}

void MineInfoDlg::readPropertyData()
{
    AcDbObjectId objId;
    SingleDataObjectHelper::GetObjectId( _T( "����Ϣ" ), objId );

    DataHelper::GetPropertyData( objId, _T( "����" ), m_name );
    DataHelper::GetPropertyData( objId, _T( "�������Ź�˾" ), m_company );

    GetTimeFromData( objId, _T( "�������" ), m_designDate );
    GetTimeFromData( objId, _T( "Ͷ������" ), m_workDate );

    DataHelper::GetPropertyData( objId, _T( "�����" ), m_quality );
    DataHelper::GetPropertyData( objId, _T( "�ܽ�����" ), m_totalQ );

    m_gasLevel.AddString( _T( "����˹" ) );
    m_gasLevel.AddString( _T( "����˹" ) );
    m_gasLevel.AddString( _T( "ú����˹ͻ��" ) );
    CString v1;
    DataHelper::GetPropertyData( objId, _T( "��˹�ȼ�" ), v1 );
    int gl = 0;
    ArxUtilHelper::StringToInt( v1, gl );
    gl = gl % 3; // ��Χ��0-2
    m_gasLevel.SetCurSel( gl );

    m_vent.AddString( _T( "���ʽ" ) );
    m_vent.AddString( _T( "ѹ��ʽ" ) );
    CString v2;
    DataHelper::GetPropertyData( objId, _T( "ͨ�緽ʽ" ), v2 );
    int vt = 0;
    ArxUtilHelper::StringToInt( v2, vt );
    vt = vt % 2; // ��Χ��{0, 1}
    m_vent.SetCurSel( vt );

    m_slider1.SetRange( 115, 190, TRUE ); // 1.15 ~ 1.9, �Ŵ�100��
    m_slider2.SetRange( 115, 130, TRUE ); // 1.15 ~ 1.3, �Ŵ�100��

    CString v3;
    DataHelper::GetPropertyData( objId, _T( "ͨ��ϵ��" ), v3 );
    double c3 = 0;
    ArxUtilHelper::StringToDouble( v3, c3 );
    int pos3 = ( int )( c3 * 100 );
    initSlider( m_slider1, 115, 190, pos3, IDC_MINE_INFO_COEFF1 );

    CString v4;
    DataHelper::GetPropertyData( objId, _T( "ͨ�����ϵ��" ), v4 );
    double c4 = 0;
    ArxUtilHelper::StringToDouble( v4, c4 );
    int pos4 = ( int )( c4 * 100 );
    initSlider( m_slider2, 115, 130, pos4, IDC_MINE_INFO_COEFF2 );

    UpdateData( FALSE );
}

void MineInfoDlg::writePropertyData()
{
    UpdateData( TRUE );

    AcDbObjectId objId;
    SingleDataObjectHelper::GetObjectId( _T( "����Ϣ" ), objId );

    DataHelper::SetPropertyData( objId, _T( "����" ), m_name );
    DataHelper::SetPropertyData( objId, _T( "�������Ź�˾" ), m_company );

    SetTimeToData( objId, _T( "�������" ), m_designDate );
    SetTimeToData( objId, _T( "Ͷ������" ), m_workDate );

    DataHelper::SetPropertyData( objId, _T( "�����" ), m_quality );
    DataHelper::SetPropertyData( objId, _T( "�ܽ�����" ), m_totalQ );

    CString v1;
    v1.Format( _T( "%d" ), m_gasLevel.GetCurSel() );
    DataHelper::SetPropertyData( objId, _T( "��˹�ȼ�" ), v1 );

    CString v2;
    v2.Format( _T( "%d" ), m_vent.GetCurSel() );
    DataHelper::SetPropertyData( objId, _T( "ͨ�緽ʽ" ), v2 );

    CString v3;
    GetDlgItem( IDC_MINE_INFO_COEFF1 )->GetWindowText( v3 );
    DataHelper::SetPropertyData( objId, _T( "ͨ��ϵ��" ), v3 );

    CString v4;
    GetDlgItem( IDC_MINE_INFO_COEFF2 )->GetWindowText( v4 );
    DataHelper::SetPropertyData( objId, _T( "ͨ�����ϵ��" ), v4 );
}

void MineInfoDlg::initSlider( CSliderCtrl& sl, int minR, int maxR, int pos, long id )
{
    if( pos < minR || pos > maxR ) pos = minR;
    sl.SetPos( pos );
    updateCoeff( id, pos );
}

void MineInfoDlg::updateCoeff( long id, int v )
{
    CString str;
    str.Format( _T( "%.2f" ), v * 0.01 );
    GetDlgItem( id )->SetWindowText( str );
}

void MineInfoDlg::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
    switch( pScrollBar->GetDlgCtrlID() )
    {
    case IDC_MINE_INFO_SLIDER1:
        updateCoeff( IDC_MINE_INFO_COEFF1, m_slider1.GetPos() );
        break;

    case IDC_MINE_INFO_SLIDER2:
        updateCoeff( IDC_MINE_INFO_COEFF2, m_slider2.GetPos() );
        break;
    }

    CDialog::OnHScroll( nSBCode, nPos, pScrollBar );
}