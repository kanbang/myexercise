#include "StdAfx.h"
#include "MFCPropertyGridCtrlHelper.h"
#include "CustomProperties.h"

#define bool2BOOL(b) (b?1:0)

MFCPropertyGridCtrlHelper::MFCPropertyGridCtrlHelper( CMFCPropertyGridCtrl* pWndPropList )
    : m_pWndPropList( NULL )
{
    if( pWndPropList != 0 )
    {
        m_pWndPropList = pWndPropList;
        m_pWndPropList->EnableHeaderCtrl( TRUE, _T( "����" ), _T( "ֵ" ) );
        m_pWndPropList->EnableDescriptionArea();
        m_pWndPropList->SetVSDotNetLook();
        m_pWndPropList->MarkModifiedProperties();
    }
}

static void SetAndAddToPropertyList( CMFCPropertyGridCtrl* pPropList, CMFCPropertyGridProperty* pProp, bool bEnable, bool bEditable, bool isBoolType = false )
{
    pProp->Enable( bool2BOOL( bEnable ) );
    if( !isBoolType ) pProp->AllowEdit( bool2BOOL( bEditable ) );
    pPropList->AddProperty( pProp );
}

void MFCPropertyGridCtrlHelper::addStringProperty( const CString& name, const CString& value, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    // VT_BSTR
    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( name, ( COleVariant )value, descr );
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addIntProperty( const CString& name, int value, int nMinValue, int nMaxValue, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    // VT_I4
    if( value < nMinValue || value > nMaxValue ) value = nMinValue;
    CMFCPropertyGridProperty* pProp = new CIntProp( name, ( long )value, nMinValue, nMaxValue, descr );
    pProp->EnableSpinControl( TRUE, nMinValue, nMaxValue ); // ʹ��spin contrl(΢���ؼ�)
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addDoubleProperty( const CString& name, double value, double dMinValue, double dMaxValue, unsigned short precise, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    // VT_R8
    if( value < dMinValue || value > dMaxValue ) value = dMinValue;
    CMFCPropertyGridProperty* pProp = new CNumericProp( name, value, dMinValue, dMaxValue, precise, descr );
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addBoolProperty( const CString& name, bool value, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );

    /*
    // VT_BOOL
    COleVariant v((short)value, VT_BOOL);
    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(name, v, descr);
    SetAndAddToPropertyList(m_pWndPropList, pProp, bEnable, false, true); // ���bool���ͣ�bEditable������Ч
    */
    // ��IntStrPropģ��
    // ��Ҫ��Ϊ��ʹ�ú���(��/��)�����Ĭ�ϵ�boolӢ��(true/false)
    AcDbIntArray intValues;
    intValues.append( 1 );
    intValues.append( 0 );
    AcStringArray strValues;
    strValues.append( _T( "��" ) );
    strValues.append( _T( "��" ) );
    IntStrProp* pProp = new IntStrProp( name, ( value ? 1 : 0 ), intValues, strValues, descr );
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, false ); // ��겻�ɱ༭
}

void MFCPropertyGridCtrlHelper::addDateTimeProperty( const CString& name, const COleDateTime& value, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );

    DateTimeProperty* pProp = new DateTimeProperty( name, ( COleVariant )value, descr );
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addStringPropertyList( const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    if( values.isEmpty() ) return;

    CString	v = value;
    if( !values.contains( value ) )
    {
        v = values.first().kACharPtr(); // ��������ڣ�Ĭ��ʹ���б��еĵ�1��
    }

    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( name, ( COleVariant )v, descr );
    int len = values.length();
    for( int i = 0; i < len; i++ )
    {
        pProp->AddOption( values[i].kACharPtr() );
    }
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, false ); // ��겻�ɱ༭
}

void MFCPropertyGridCtrlHelper::addIntPropertList( const CString& name, int value, const AcDbIntArray& intValues, const AcStringArray& strValues, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    if( intValues.isEmpty() || strValues.isEmpty() || intValues.length() != strValues.length() ) return;

    if( !intValues.contains( value ) )
    {
        value = intValues[0]; // ��������ڣ�Ĭ��ʹ���б��еĵ�1��
    }

    IntStrProp* pProp = new IntStrProp( name, value, intValues, strValues, descr );
    //COleVariant v = pProp->GetValue();
    //assert(v.vt == VT_INT); // false
    //assert(v.vt == VT_I2); // true
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, false ); // ��겻�ɱ༭
}

void MFCPropertyGridCtrlHelper::addDataObjectPropertyList( const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    if( values.isEmpty() ) return;

    CString	v = value;
    if( !values.contains( value ) )
    {
        //v = values.first().kACharPtr(); // ��������ڣ�Ĭ��ʹ���б��еĵ�1��
        v = _T( "" ); // �հ�
    }

    DataObjectProperty* pProp = new DataObjectProperty( name, v, values, descr );
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, false ); // ��겻�ɱ༭
}

// ���������, ��һ�����Ե����"..."��ť
// ������Կ��ǲ�������ί�еķ���(Ҳ�ɳ�Ϊ�ص�)��������
// ���磺��Ͳ��������, �����ť�����Ի����������
// ��ť�൱��һ��"������"
// Ŀǰ�Ĵ���:
//     1) �������Բ���"�ֶι���Ի���"��"�ֶ���Ϣ�Ի���"����ʾ
//     2) ֻ����ͨ������������, ��������MFCPropertyGridCtrlHelper��������һ������
// ��ϸ���÷�ʾ���μ�PropertyDataUpdater.cpp�е�BuildPropList2()��ע�Ͳ���
// �����д�������׶Σ���ʱ����������Ŀ��ʹ��ClickProp
class CustomClickProp : public CMFCPropertyGridProperty
{
public:
    CustomClickProp( const CString& strName, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR lpszDescr = NULL );

protected:
    virtual BOOL HasButton() const
    {
        return TRUE;
    }
    virtual void OnClickButton( CPoint point );

private:
    ClickPropModifyCallBack m_cpmcb;
};

CustomClickProp::CustomClickProp( const CString& strName, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR lpszDescr )
    : CMFCPropertyGridProperty( strName, ( COleVariant )value, lpszDescr ), m_cpmcb( cpmcb )
{
    //AllowEdit(FALSE);
}

void CustomClickProp::OnClickButton( CPoint /*point*/ )
{
    //AfxMessageBox(_T("Show your dialog here..."));
    CString newValue;
    bool ret = ( *m_cpmcb )( GetValue(), newValue );
    if( ret ) SetValue( ( COleVariant )newValue );
}

void MFCPropertyGridCtrlHelper::addClickProperty( const CString& name, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    ASSERT_VALID( cpmcb != 0 );

    CustomClickProp* pProp = new CustomClickProp( name, ( COleVariant )value, cpmcb, descr );
    SetAndAddToPropertyList( m_pWndPropList, pProp, bEnable, false ); // ��겻�ɱ༭
}
