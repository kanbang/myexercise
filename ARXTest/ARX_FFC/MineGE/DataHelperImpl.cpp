#include "StdAfx.h"
#include "DataHelperImpl.h"

#include "FieldHelper.h"

static void DealSpecialString( CString& value )
{
    // ɾ����β�հ�
    value.Trim();

    // ����"null"�ַ���
    if( value.CompareNoCase( _T( "NULL" ) ) == 0 )
    {
        value = _T( "" );
    }
}

DataHelperImpl::DataHelperImpl ( DataObject* pObj ) : m_pObj( pObj )
{
}

bool DataHelperImpl::getPropertyData( const CString& fieldName, CString& value )
{
    //m_pObj->assertReadEnabled();

    int index = FieldHelper::FindField( m_pObj->getType(), fieldName );
    if( index < 1 ) return false; // �ֶβ�����

    // �ֶη��ص�������1��ʼ
    // �����ݵ�������0��ʼ
    bool ret = m_pObj->getData( index - 1, value );
    if( ret )
    {
        // ����һЩ�����ַ�
        DealSpecialString( value );
    }
    return ret;
}

bool DataHelperImpl::setPropertyData( const CString& fieldName, const CString& value )
{
    //m_pObj->assertWriteEnabled();

    int index = FieldHelper::FindField( m_pObj->getType(), fieldName );
    if( index < 1 ) return false; // �ֶβ�����

    // ����һЩ�ַ�
    CString vv( value );
    DealSpecialString( vv );

    // �ֶη��ص�������1��ʼ
    // �����ݵ�������0��ʼ
    return m_pObj->setData( index - 1, vv );
}
