#include "StdAfx.h"
#include "DataReader.h"

DataReader::DataReader( CDaoDatabase* pDB )
{
    m_pRecordSet = new CDaoRecordset( pDB );
}
DataReader::~DataReader( void )
{
    delete m_pRecordSet;
}

void DataReader::beginReadData( const CString& tableName )
{
    // Open打开不存在的表可能出现异常
    // 目前并没有考虑
    CString sql = _T( "select * from " ) + tableName;
    m_pRecordSet->Open( dbOpenDynaset, sql, dbReadOnly ); // 只读(但允许其它用户修改)
    m_pRecordSet->MoveFirst();
}

void DataReader::endReadData()
{
    m_pRecordSet->Close();
}

bool DataReader::hasNext() const
{
    return !( m_pRecordSet->IsEOF() );
}

void DataReader::moveNext()
{
    m_pRecordSet->MoveNext();
}

void DataReader::readStringData( const CString& fieldName, CString& value )
{
    COleVariant v;
    m_pRecordSet->GetFieldValue( fieldName, v );
    value = v.bstrVal;
}

void DataReader::readIntData( const CString& fieldName, int& value )
{
    COleVariant v;
    m_pRecordSet->GetFieldValue( fieldName, v );
    value = v.intVal;
}

void DataReader::readRealData( const CString& fieldName, double& value )
{
    COleVariant v;
    m_pRecordSet->GetFieldValue( fieldName, v );
    value = v.dblVal;
}

static bool Short2Bool( short value )
{
    return value ? true : false;
}

void DataReader::readBoolData( const CString& fieldName, bool& value )
{
    COleVariant v;
    m_pRecordSet->GetFieldValue( fieldName, v );
    value = Short2Bool( v.boolVal );
}