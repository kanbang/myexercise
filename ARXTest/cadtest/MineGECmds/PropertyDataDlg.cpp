#include "stdafx.h"
#include "PropertyDataDlg.h"

#include "../MineGE/DataHelper.h"
#include "../MineGE/FieldHelper.h"
#include "../MineGE/PropertyDataUpdater.h"

IMPLEMENT_DYNAMIC( PropertyDataDlg, CDialog )

PropertyDataDlg::PropertyDataDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( PropertyDataDlg::IDD, pParent )
{
    m_showAll = true; // Ĭ����ʾȫ������
}

PropertyDataDlg::~PropertyDataDlg()
{
}

void PropertyDataDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( PropertyDataDlg, CDialog )
    ON_BN_CLICKED( IDOK, &PropertyDataDlg::OnBnClickedOk )
END_MESSAGE_MAP()


// PropertyDataDlg ��Ϣ�������

BOOL PropertyDataDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ����, ��λ, ��ʾCMFCPropertyGridCtrl
    CRect rect;
    GetDlgItem( IDC_PROP_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );
    m_propertyDataList.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, ( UINT ) - 1 );

    // ��ȡ��������
    CString type;
    DataHelper::GetTypeName( m_objId, type );

    // ��ȡ�ֶ�
    AcStringArray fileds;
    if( m_showAll )
    {
        FieldHelper::GetAllFields( type, fileds );
    }
    else
    {
        fileds.append( m_fields );
    }

    // �����������ݿؼ�
    PropertyDataUpdater::BuildPropGridCtrl( &m_propertyDataList, type, fileds );

    // �����������
    bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );

    // �������ʧ��
    if( !ret )
    {
        m_propertyDataList.EnableWindow( FALSE );
        m_propertyDataList.ShowWindow( SW_HIDE );
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void PropertyDataDlg::OnBnClickedOk()
{
    UpdateData( TRUE ); // ���¿ؼ�

    // ����ͼԪ����������
    PropertyDataUpdater::WriteDataToGE( &m_propertyDataList, m_objId );

    OnOK();
}

void PropertyDataDlg::setMineGE( const AcDbObjectId& objId )
{
    m_objId = objId;
}

void PropertyDataDlg::addField( const CString& field )
{
    m_fields.append( field );
}

void PropertyDataDlg::showAllData( bool bFlag )
{
    m_showAll = bFlag;
}