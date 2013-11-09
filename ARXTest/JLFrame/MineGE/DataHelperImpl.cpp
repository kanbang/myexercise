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
    // FindField()�������ص�������1��ʼ
    int index = FieldHelper::FindField( m_pObj->getType(), fieldName );
    if( index < 1 ) return false; // �ֶβ�����

    // ���ݶ���(DataObject��ʵ�ʾ���һ���䳤�ַ�������)��������0��ʼ
    // ��ˣ���Ҫ��ȥ1
    bool ret = m_pObj->getData( index - 1, value );
    if( ret )
    {
        // ����һЩ�����ַ�(���磺NULL)
        DealSpecialString( value );
    }
    return ret;
}

bool DataHelperImpl::setPropertyData( const CString& fieldName, const CString& value )
{
    // FindField()�������ص�������1��ʼ
    int index = FieldHelper::FindField( m_pObj->getType(), fieldName );
    if( index < 1 ) return false; // �ֶβ�����

    // ����һЩ�ַ�(���磺NULL)
    CString vv( value );
    DealSpecialString( vv );

    // ���ݶ���(DataObject��ʵ�ʾ���һ���䳤�ַ�������)��������0��ʼ
    // ��ˣ���Ҫ��ȥ1
    return m_pObj->setData( index - 1, vv );
}
