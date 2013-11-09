#include "stdafx.h"
#include "InstrumentDlg.h"

#include "SimpleInputDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

IMPLEMENT_DYNAMIC( InstrumentDlg, CDialog )

InstrumentDlg::InstrumentDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( InstrumentDlg::IDD, pParent )
    , m_type( _T( "" ) )
    , m_num( _T( "" ) )
    , m_factory( _T( "" ) )
    , m_institute( _T( "" ) )
    , m_ok( FALSE )
    //, m_name(_T(""))
{

}

InstrumentDlg::~InstrumentDlg()
{
}

void InstrumentDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_INSTRUMENT_LIST, m_list );
    //DDX_Text(pDX, IDC_INSTRUMENT_NAME, m_name);
    DDX_Text( pDX, IDC_INSTRUMENT_TYPE, m_type );
    DDX_Text( pDX, IDC_INSTRUMENT_NUM, m_num );
    DDX_Text( pDX, IDC_INSTRUMENT_FACTORY, m_factory );
    DDX_Text( pDX, IDC_INSTRUMENT_INSTITUTE, m_institute );
    DDX_Radio( pDX, IDC_INSTRUMENT_RADIO1, m_ok );
}

BEGIN_MESSAGE_MAP( InstrumentDlg, CDialog )
    ON_WM_CLOSE()
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_INSTRUMENT_LIST, &InstrumentDlg::OnLvnItemchangedInstrumentList )
    ON_BN_CLICKED( IDC_INSTRUMENT_ADD_BTN, &InstrumentDlg::OnBnClickedInstrumentAddBtn )
    ON_BN_CLICKED( IDC_INSTRUMENT_DEL_BTN, &InstrumentDlg::OnBnClickedInstrumentDelBtn )
END_MESSAGE_MAP()


// InstrumentDlg ��Ϣ�������

BOOL InstrumentDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
    CRect rect;
    m_list.GetClientRect( &rect );
    double c = 0.3;
    m_list.InsertColumn( 0, _T( "���" ), LVCFMT_LEFT, rect.Width()*c );
    m_list.InsertColumn( 1, _T( "����" ), LVCFMT_LEFT, rect.Width() * ( 1 - c ) );

    clearAll();
    initList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void InstrumentDlg::OnClose()
{
    updateLastSelect();

    CDialog::OnClose();
}

void InstrumentDlg::OnLvnItemchangedInstrumentList( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    if( pNMLV->iItem != -1 )
    {
        // �ڼ���ͼԪ
        int row = pNMLV->iItem;

        if( m_lastPos != -1 )
        {
            // ��������
            UpdateData( TRUE );
            writePropertyData( m_objIds[m_lastPos] );
        }
        // ��ȡ��ǰ���ͼԪ������
        readPropertyData( m_objIds[row] );
        UpdateData( FALSE ); // �������ݵ�����

        m_lastPos = row;
    }

    *pResult = 0;
}

void InstrumentDlg::initList()
{
    AcStringArray names;
    DataObjectListHelper::GetObjectNameList( _T( "��ȫ����Ǳ�" ), names );

    int len = names.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObjectId objId;
        DataObjectListHelper::GetObjectId( _T( "��ȫ����Ǳ�" ), names[i].kACharPtr(), objId );
        m_objIds.append( objId );

        // ���
        CString num;
        num.Format( _T( "%d" ), i + 1 );
        m_list.InsertItem( i, num );
        //m_list.SetItemData(i, i);

        m_list.SetItemText( i, 0, num );
        m_list.SetItemText( i, 1, names[i].kACharPtr() );
    }
    // ѡ�е�1��
    if( len > 0 ) m_list.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
}

void InstrumentDlg::updateLastSelect()
{
    int row = LB_ERR;
    for( int i = 0; i < m_list.GetItemCount(); i++ )
    {
        if( LVIS_SELECTED == m_list.GetItemState( i, LVIS_SELECTED ) )
        {
            row = i;
            break;
        }
    }

    if( row != LB_ERR )
    {
        UpdateData( TRUE );
        writePropertyData( m_objIds[row] );
    }
}

void InstrumentDlg::readPropertyData( const AcDbObjectId& objId )
{
    //DataHelper::GetPropertyData(objId, _T("����"), m_name);
    DataHelper::GetPropertyData( objId, _T( "�ͺ�" ), m_type );
    DataHelper::GetPropertyData( objId, _T( "����" ), m_num );
    DataHelper::GetPropertyData( objId, _T( "������λ" ), m_factory );
    DataHelper::GetPropertyData( objId, _T( "�������鵥λ" ), m_institute );

    CString v2;
    DataHelper::GetPropertyData( objId, _T( "����������" ), v2 );
    int i2 = 0;
    ArxUtilHelper::StringToInt( v2, i2 );
    i2  = i2 % 2;
    m_ok = i2; // 0-���ϸ�1-�ϸ�
}

void InstrumentDlg::writePropertyData( const AcDbObjectId& objId )
{
    //DataHelper::SetPropertyData(objId, _T("����"), m_name);
    DataHelper::SetPropertyData( objId, _T( "�ͺ�" ), m_type );
    DataHelper::SetPropertyData( objId, _T( "����" ), m_num );
    DataHelper::SetPropertyData( objId, _T( "������λ" ), m_factory );
    DataHelper::SetPropertyData( objId, _T( "�������鵥λ" ), m_institute );

    CString v2;
    v2.Format( _T( "%d" ), m_ok ); // 0-���ϸ�1-�ϸ�
    DataHelper::SetPropertyData( objId, _T( "����������" ), v2 );
}

void InstrumentDlg::clearAllData()
{
    m_type = _T( "" );
    m_num = _T( "" );
    m_factory = _T( "" );
    m_institute = _T( "" );
    m_ok = FALSE;
    //m_name = _T("");
}

void InstrumentDlg::clearAll()
{
    m_list.DeleteAllItems();
    m_objIds.removeAll();
    m_lastPos = -1;

    clearAllData();
}

static bool GetInputName( CString& name )
{
    // �л���Դ
    CAcModuleResourceOverride myResources;

    SimpleInputDlg sidlg;
    if( sidlg.DoModal() != IDOK ) return false;

    name = sidlg.m_str;
    return true;
}

void InstrumentDlg::OnBnClickedInstrumentAddBtn()
{
    CString name;
    if( !GetInputName( name ) ) return;

    DataObjectListHelper::AddObject( _T( "��ȫ����Ǳ�" ), name );

    // ���ñȽϱ��ķ�����ÿ�ε��"����"��ť�����list������������
    updateLastSelect();
    clearAll();
    initList();
}

void InstrumentDlg::OnBnClickedInstrumentDelBtn()
{
    int row = LB_ERR;
    for( int i = 0; i < m_list.GetItemCount(); i++ )
    {
        if( LVIS_SELECTED == m_list.GetItemState( i, LVIS_SELECTED ) )
        {
            row = i;
            break;
        }
    }
    CString name = m_list.GetItemText( row, 1 );
    DataObjectListHelper::RemoveObject( _T( "��ȫ����Ǳ�" ), name );
    clearAll();
    initList();
}
