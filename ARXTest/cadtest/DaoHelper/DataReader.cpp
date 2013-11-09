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
    // Open�򿪲����ڵı���ܳ����쳣
    // Ŀǰ��û�п���
    CString sql = _T( "select * from " ) + tableName;
    m_pRecordSet->Open( dbOpenDynaset, sql, dbReadOnly ); // ֻ��(�����������û��޸�)
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