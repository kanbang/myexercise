#include "stdafx.h"
#include "PropertyDataDlg2.h"

#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC( PropertyDataDlg2, CDialog )

PropertyDataDlg2::PropertyDataDlg2( CWnd* pParent /*=NULL*/ )
    : CDialog( PropertyDataDlg2::IDD, pParent )
{

}

PropertyDataDlg2::~PropertyDataDlg2()
{
}

void PropertyDataDlg2::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_PROP_LIST, m_propertyDataList );
}


BEGIN_MESSAGE_MAP( PropertyDataDlg2, CDialog )
    ON_BN_CLICKED( IDOK, &PropertyDataDlg2::OnBnClickedOk )
END_MESSAGE_MAP()


// PropertyDataDlg2 ��Ϣ�������

void PropertyDataDlg2::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    OnOK();
}

BOOL PropertyDataDlg2::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_propertyDataList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

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

void PropertyDataDlg2::setMineGE( const AcDbObjectId& objId )
{
    m_objId = objId;
}

void PropertyDataDlg2::addField( const CString& field )
{
    m_fields.append( field );
}

void PropertyDataDlg2::showAllData( bool bFlag )
{
    m_showAll = bFlag;
}