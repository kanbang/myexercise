#include "stdafx.h"
#include "FieldManagerDlg.h"

#include "InputDlg.h"

#include "../MineGE/FieldHelper.h"
#include "../MineGE/FieldInfoHelper.h"
//#include "../MineGE/TypeNameMapHelper.h"

#include "../ArxHelper/HelperClass.h"

IMPLEMENT_DYNAMIC( FieldManagerDlg, CDialog )

FieldManagerDlg::FieldManagerDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( FieldManagerDlg::IDD, pParent )
{

}

FieldManagerDlg::~FieldManagerDlg()
{
}

void FieldManagerDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_TYPE_COMBO2, m_typeCombo );
    DDX_Control( pDX, IDC_FIELD_LIST, m_fieldListBox );
}

BEGIN_MESSAGE_MAP( FieldManagerDlg, CDialog )
    ON_BN_CLICKED( IDC_ADD_BTN, &FieldManagerDlg::OnBnClickedAddBtn )
    ON_BN_CLICKED( IDC_DEL_BTN, &FieldManagerDlg::OnBnClickedDelBtn )
    ON_CBN_SELCHANGE( IDC_TYPE_COMBO2, &FieldManagerDlg::OnCbnSelchangeTypeCombo2 )
    ON_BN_CLICKED( IDC_APPLY_BTN, &FieldManagerDlg::OnBnClickedApplyBtn )
    ON_LBN_SELCHANGE( IDC_FIELD_LIST, &FieldManagerDlg::OnLbnSelchangeFieldList )
    ON_WM_CLOSE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL FieldManagerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��ʾ�ֶ���Ϣ�ӶԻ���
    CRect rect;
    GetDlgItem( IDC_FIELD_INFO_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );
    fidlg.Create( IDD_FIELD_INFO_DLG, this );
    fidlg.MoveWindow( &rect );
    fidlg.ShowWindow( SW_SHOW );

    fillContent(); // �������

    return TRUE;
}

void FieldManagerDlg::OnClose()
{
    clearFieldInfoArray();
    CDialog::OnClose();
}

void FieldManagerDlg::OnBnClickedAddBtn()
{
    InputDlg dlg;
    if( dlg.DoModal() != IDOK ) return;

    // ע��m_lastSel�ĸ�������
    if( m_fieldListBox.GetCount() == 0 ) setLastSelIndex( 0 );

    if( !isValidField( dlg.m_str ) )
    {
        CString msg;
        msg.Format( _T( "�Ƿ����ֶ�\n[%s]" ), dlg.m_str );
        MessageBox( msg );
        return;
    }
    if( isFieldExistInListBox( dlg.m_str ) )
    {
        CString msg;
        msg.Format( _T( "�ֶ�[%s]�Ѵ���!" ), dlg.m_str );
        MessageBox( msg );
        return;
    }

    // ����ֶε�listbox��
    int index = m_fieldListBox.AddString( dlg.m_str );
    if( index != LB_ERR )
    {
        m_fieldListBox.SetCurSel( index );
        // ***�����µ��ֶ���Ϣ������¼***
        m_infoes.append( new FieldInfo() );

        // �л�selection, �����ϴ�selection����λ�õ��ֶ���Ϣ
        updateFieldInfo();
    }
}

void FieldManagerDlg::OnBnClickedDelBtn()
{
    if( m_fieldListBox.GetCount() == 0 ) return;

    if( IDCANCEL == MessageBox(
                _T( "ɾ���ֶε�ͬʱҲ��ɾ��������Ӧ����ͼԪ���ֶκ�����\n\nȷ��Ҫɾ���ֶ��𣿡������ز���!!!��" ),
                _T( "��ʾ��" ),
                MB_ICONEXCLAMATION | MB_ICONWARNING | MB_OKCANCEL ) ) return;

    int index = m_fieldListBox.GetCurSel();
    if( index == LB_ERR ) return;

    m_fieldListBox.DeleteString( index );

    // ****ͬʱɾ����Ӧ���ֶ���Ϣ****
    delete m_infoes[index];
    m_infoes.removeAt( index );

    // ע��m_lastSel�ĸ�������
    if( m_fieldListBox.GetCount() == 0 ) setLastSelIndex( LB_ERR );
}

void FieldManagerDlg::OnCbnSelchangeTypeCombo2()
{
    // ������֮ǰ�������޸�***???

    // ����ֶ��б�
    clearFieldListBox();

    // ����ֶ�
    fillFieldListBox( getSelType() );

    // �����ֶ���Ϣ�Ի������ʾ
    showFieldInfo();

    // Invalidate()�������Ի���ʧЧ
    // ��InvalidateRect����IDC_DRAW_POS���ھ��ο�ʧЧ
    // �Ӷ������Ч��
    CRect rect;
    GetDlgItem( IDC_DRAW_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );
    InvalidateRect( &rect );

    // ���»�ͼ������ʾ
    // ������UpdateWindow()Ҳ�ܹ�����WM_PAINT��Ϣ���Ӷ�����OnPaint()����
    //UpdateWindow();
}

void FieldManagerDlg::OnLbnSelchangeFieldList()
{
    // �л�֮ǰ�����ϴ�ѡ����ֶ���Ϣ
    // ������ֶ���Ϣ����Ч��
    updateFieldInfo();
}

/*
 * ������
 *	  1) �õ�ԭʼ���ֶ�����L1���ֶ��б���е��ֶ�����L2
 *    2) ��������L2,�ж��ֶ��Ƿ������L1��
 *       a) ������ڣ�����ֶα��ֲ��䣬����L1���Ƴ����ֶΣ�
 *       b) ��������ڣ���ת��(3)��
 *    3) ���ֶ�������ӵ��ֶΣ���ִ��"�����ֶ�"����Add
 *    4) ��ɱ���L2�����ʣ�µ�L1��Ԫ�ؾ�����Ҫɾ�����ֶ�
 *    5) ִ��"ɾ���ֶ�"����Remove
 */
void FieldManagerDlg::OnBnClickedApplyBtn()
{
    if( m_fieldListBox.GetCount() > 0 )
    {
        // �л�֮ǰ�����ϴ�ѡ����ֶ���Ϣ
        // ������ֶ���Ϣ����Ч��
        if( !updateFieldInfo() ) return;
    }

    // ѡ���ͼԪ����
    CString selType = getSelType();
    if( selType.GetLength() == 0 ) return;

    AcStringArray fields;
    FieldHelper::GetAllFields( selType, fields );
    if( fields.isEmpty() && m_fieldListBox.GetCount() == 0 )
    {
        MessageBox( _T( "û���ֶοɸ���" ) );
        return;
    }

    // "ʣ��"�ֶ�
    // ��m_infoesӦ����һһ��Ӧ��
    AcStringArray leftFields;
    for( int i = 0; i < m_fieldListBox.GetCount(); i++ )
    {
        CString text;
        m_fieldListBox.GetText( i, text );
        leftFields.append( text );
    }

    int len = leftFields.length();
    for( int i = 0; i < len; i++ )
    {
        CString field = leftFields[i].kACharPtr();
        int index = fields.find( field );
        if( index >= 0 ) // �Ѵ��ڣ�����
        {
            fields.removeAt( index );
        }
        else
        {
            // �����ֶ�
            FieldHelper::AddField( selType, field );
        }
        // Ĭ���������ò���Ҫ��ӵ��ʵ���
        if( m_infoes[i]->isDefault() )
        {
            FieldInfoHelper::RemoveFieldInfo( selType, field );
        }
        else
        {
            // �������ֶ�
            if( index < 0 || !FieldInfoHelper::FindFieldInfo( selType, field ) )
            {
                FieldInfoHelper::AddFieldInfo( selType, field, *m_infoes[i] );
            }
            else
            {
                // �������е��ֶ���Ϣ
                FieldInfoHelper::WriteFieldInfo( selType, field, *m_infoes[i] );
            }
        }
    }

    // ɾ���ֶ�
    len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        FieldHelper::RemoveField( selType, fields[i].kACharPtr() );
    }

    MessageBox( _T( "�ֶ���Ϣ���³ɹ�!" ) );
}

bool FieldManagerDlg::updateFieldInfo()
{
    fidlg.UpdateData( TRUE );
    bool ret = fidlg.validateFieldInfo();
    if( ret )
    {
        if( getLastSelIndex() != LB_ERR )
        {
            fidlg.writeToFieldInfo( *m_infoes[getLastSelIndex()] );
        }
        showFieldInfo();
    }
    else
    {
        m_fieldListBox.SetCurSel( getLastSelIndex() );
    }
    return ret;
}

CString FieldManagerDlg::getSelType()
{
    CString selType;
    int sel = m_typeCombo.GetCurSel();
    if( sel != CB_ERR )
    {
        m_typeCombo.GetLBText( sel, selType );
    }
    return selType;
}

CString FieldManagerDlg::getSelField()
{
    CString selField;
    int sel = m_fieldListBox.GetCurSel();
    if( sel != LB_ERR )
    {
        m_fieldListBox.GetText( sel, selField );
    }
    return selField;
}

void FieldManagerDlg::clearFieldInfoArray()
{
    // ����ֶ���Ϣ(ע���ͷ��ڴ�)
    int len = m_infoes.length();
    for( int i = 0; i < len; i++ )
    {
        delete m_infoes[i];
    }
    m_infoes.removeAll();
}

void FieldManagerDlg::clearFieldListBox()
{
    m_fieldListBox.ResetContent();

    // ����ֶ���Ϣ
    clearFieldInfoArray();
}

void FieldManagerDlg::fillFieldListBox( const CString& type )
{
    setLastSelIndex( LB_ERR ); // ��¼listbox�л�֮ǰ������λ��

    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );
    if( fields.isEmpty() ) return;

    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        CString field = fields[i].kACharPtr();
        m_fieldListBox.AddString( field );

        // ��ʼΪĬ������
        FieldInfo* pInfo = new FieldInfo();
        // ��ȡ�ֶ���Ϣ
        FieldInfoHelper::ReadFieldInfo( type, field, *pInfo );
        m_infoes.append( pInfo );
    }
    m_fieldListBox.SetCurSel( 0 );
}

bool FieldManagerDlg::isFieldExistInListBox( const CString& field )
{
    return ( m_fieldListBox.FindString( 0, field ) != LB_ERR );
}

bool FieldManagerDlg::isValidField( const CString& field )
{
    CString tempStr( field );
    tempStr.Trim();

    // �ų����ַ���
    if( tempStr.GetLength() == 0 ) return false;

    // �ų��Ƿ��ַ�������
    // {. * & @ # $ % ...}
    if( tempStr.Find( _T( '.' ) ) >= 0 ) return false;

    return true;
}

void FieldManagerDlg::showFieldInfo()
{
    int sel = m_fieldListBox.GetCurSel();
    if( sel != LB_ERR )
    {
        FieldInfo* pInfo = m_infoes[sel];
        fidlg.readFromFieldInfo( *pInfo );
        fidlg.UpdateData( FALSE );

        setLastSelIndex( sel ); // ��¼"��һ��"������λ��
    }
}

void FieldManagerDlg::fillContent()
{
    // ������е�ͼԪ����������
    m_typeCombo.ResetContent();
    m_fieldListBox.ResetContent();

    AcStringArray types;
    FieldHelper::GetAllRegTypes( types );
    if( types.isEmpty() ) return;

    int len = types.length();
    //acutPrintf(_T("\n�ܸ���:%d"), len);
    for( int i = 0; i < len; i++ )
    {
        m_typeCombo.AddString( types[i].kACharPtr() );
        //acutPrintf(_T("\n%d��%s"), i, types[i].kACharPtr());
    }

    // ��λ��1��ͼԪ
    m_typeCombo.SetCurSel( 0 );

    // ����1�������������������ֶ�
    fillFieldListBox( types[0].kACharPtr() );

    // ��ʾ��1���ֶ���Ϣ
    showFieldInfo();
}

void FieldManagerDlg::OnPaint()
{
    CPaintDC dc( this ); // device context for painting

    CRect rect;
    GetDlgItem( IDC_DRAW_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );

    CString type = getSelType();
    if( type.GetLength() != 0 )
    {
        CString name( type );
        //TypeNameMapHelper::GetTypeNameMap(type, name);
        ArxClassHelper::GetDxfName( type, name );
        CString text;
        text.Format( _T( "DXF����: %s" ), name );
        dc.DrawText( text, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
    }

    // �������Բο�CustomProperties.cpp�е�
    // void DateTimeProperty::OnDrawDescription( CDC* pDC, CRect rect ) ����ʵ��
    // Ҫ��������������Ŀ(�Ա�Ч����)
}

void FieldManagerDlg::setLastSelIndex( int sel )
{
    m_lastSel = sel;
}

int FieldManagerDlg::getLastSelIndex() const
{
    return m_lastSel;
}