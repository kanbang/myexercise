#include "StdAfx.h"
#include "DataLink.h"

#include "DataHelper.h"
#include "DataHelperImpl.h"

#include "FieldHelper.h"
#include "FieldType.h"

#include "../ArxHelper/HelperClass.h"

static resbuf* GoToListEnd( resbuf* pHead, const CString& field )
{
    resbuf* pTemp = pHead;
    while( pTemp != 0 )
    {
        // ������ͬ���ֶ�����
        if( field.CompareNoCase( pTemp->resval.rstring ) == 0 ) break;
        bool isBreak = false;
        // ѭ��3�Σ�������һ���ֶνڵ�
        for( int i = 0; i < 3; i++ )
        {
            if( pTemp->rbnext == 0 )
            {
                isBreak = true;
                break;
            }
            pTemp = pTemp->rbnext;
        }
        if( isBreak ) break;
    }
    return pTemp;
}

static void LinkGenericData( resbuf*& pHead, const CString& field, short rtype, void* pValue )
{
    if( field.GetLength() == 0 ) return;

    resbuf* pTemp = 0;
    if( pHead == 0 )
    {
        pHead = acutBuildList( RTSTR, field, RTSHORT, rtype, RTLONG_PTR, ( long )pValue, 0 );
    }
    else
    {
        pTemp = GoToListEnd( pHead, field );
        if( pTemp->rbnext == 0 )
        {
            pTemp->rbnext = acutBuildList( RTSTR, field, RTSHORT, rtype, RTLONG_PTR, ( long )pValue, 0 );
        }
    }
}

static resbuf* SetData( resbuf* pStart, const CString& value )
{
    resbuf* pTypeNode = pStart->rbnext;
    resbuf* pValueNode = pTypeNode->rbnext;

    short rtype = pTypeNode->resval.rint;
    long ptr = pValueNode->resval.mnLongPtr;
    switch( rtype )
    {
    case DT_STRING:
    {
        CString* pValue = ( CString* )ptr;
        *pValue = value;
    }
    break;

    case DT_INT:
    {
        int iv = 0;
        ArxUtilHelper::StringToInt( value, iv );
        int* pValue = ( int* )ptr;
        *pValue = iv;
    }
    break;

    case DT_NUMERIC:
    {
        double dv = 0;
        ArxUtilHelper::StringToDouble( value, dv );
        double* pValue = ( double* )ptr;
        *pValue = dv;
    }
    break;

    case DT_BOOL:
    {
        int iv = 0;
        bool bv = false;
        if( ArxUtilHelper::StringToInt( value, iv ) )
        {
            bv = ( iv != 0 ); // ����0Ϊ�٣�������0Ϊ��
        }
        bool* pValue = ( bool* )ptr;
        *pValue = bv;
    }
    break;

    case DT_DATE:
    {
        COleDateTime dt;
        if( ( value.GetLength() == 0 ) || !dt.ParseDateTime( value ) )
        {
            // ��ǰʱ��
            dt = COleDateTime::GetCurrentTime();
        }
        COleDateTime* pValue = ( COleDateTime* )ptr;
        *pValue = dt;
    }
    break;
    }
    return pValueNode->rbnext;
}

static resbuf* GetData( resbuf* pStart, CString& value )
{
    resbuf* pTypeNode = pStart->rbnext;
    resbuf* pValueNode = pTypeNode->rbnext;

    short rtype = pTypeNode->resval.rint;
    long ptr = pValueNode->resval.mnLongPtr;
    switch( rtype )
    {
    case DT_STRING:
        value = *( ( CString* )ptr );
        break;

    case DT_INT:
        ArxUtilHelper::IntToString( *( ( int* )ptr ), value );
        break;

    case DT_NUMERIC:
        ArxUtilHelper::DoubleToString( *( ( double* )ptr ), value );
        break;

    case DT_BOOL:
    {
        bool bv = *( bool* )ptr;
        value.Format( _T( "%d" ), ( bv ? -1 : 0 ) ); // -1��ʾ��, 0��ʾ��
    }
    break;

    case DT_DATE:
    {
        COleDateTime dt = *( COleDateTime* )ptr;
        value = ( COleVariant )dt;
    }
    break;
    }
    return pValueNode->rbnext;
}

DataLink::DataLink() : m_pDatasToFileds( 0 )
{

}

DataLink::~DataLink( void )
{
    if( m_pDatasToFileds != NULL )
    {
        acutRelRb( m_pDatasToFileds );
    }
}

void DataLink::setDataSource( const AcDbObjectId& objId )
{
    // ֻ�������һ��
    if( !m_objId.isNull() | objId.isNull() ) return;
    m_objId = objId;
}

AcDbObjectId DataLink::getDataSource() const
{
    return m_objId;
}

void DataLink::linkStringData( const CString& field, CString* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_STRING, pValue );
}

void DataLink::linkIntData( const CString& field, int* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_INT, pValue );
}

void DataLink::linkDoubleData( const CString& field, double* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_NUMERIC, pValue );
}

void DataLink::linkBoolData( const CString& field, bool* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_BOOL, pValue );
}

void DataLink::linkDateTimeData( const CString& field, COleDateTime* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_DATE, pValue );
}

void DataLink::initDatas()
{
    if( m_pDatasToFileds == 0 )
    {
        regDatas();
    }
}

bool DataLink::checkFields()
{
    // ��ȡ��������
    CString type;
    if( !DataHelper::GetTypeName( m_objId, type ) ) return false;

    // ��ȡ�ֶ�
    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );
    if( fields.isEmpty() ) return false;

    // �ж��ֶ��Ƿ����
    resbuf* pTemp = m_pDatasToFileds;
    while( pTemp != 0 )
    {
        // �ж��ֶ��Ƿ����
        if( !fields.contains( pTemp->resval.rstring ) )
        {
            // ������
            acutPrintf( _T( "\n����:%s\t�ֶ�:[%s]������\n" ), type, pTemp->resval.rstring );
            break;
        }
        for( int i = 0; i < 3; i++ ) pTemp = pTemp->rbnext;
    }

    // ��������ֶζ����ڣ���ôpTemp=0
    return ( pTemp == 0 );
}

static bool UpdateData_Helper( const AcDbObjectId& dObjId, resbuf* pTemp, bool save )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDb::OpenMode om = ( save ? ( AcDb::kForWrite ) : ( AcDb::kForRead ) );
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, dObjId, om ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    DataObject* pDO = DataObject::cast( pObj );
    if( pDO == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // ί��DataHelperImpl��������������
    DataHelperImpl dh( pDO );
    while( pTemp != 0 )
    {
        CString field = pTemp->resval.rstring;
        if( save )
        {
            CString value;
            pTemp = GetData( pTemp, value );
            dh.setPropertyData( field, value );
        }
        else
        {
            CString value;
            bool ret = dh.getPropertyData( field, value );
            if( !ret )
            {
                for( int i = 0; i < 3; i++ ) pTemp = pTemp->rbnext;
            }
            else
            {
                pTemp = SetData( pTemp, value );
            }
        }
    }
    actrTransactionManager->endTransaction();

    return true;
}

bool DataLink::updateData( bool save )
{
    // ��ȡ���ݶ���
    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( m_objId, dObjId ) ) return false;

    // ����ֶ�û�г�ʼ�������ʼ��
    initDatas();

    // ����ֶ��Ƿ����
    //if(!checkFields()) return false;
    checkFields();

    // ��������
    return UpdateData_Helper( dObjId, m_pDatasToFileds, save );
}