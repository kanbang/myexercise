#include "StdAfx.h"
#include "DataWriter.h"

#include <cfloat>
#include <climits>

DataWriter::DataWriter( CDaoDatabase* pDB ) : m_pDB( pDB )
{
    m_pRecordSet = new CDaoRecordset( pDB );
}

DataWriter::~DataWriter( void )
{
    delete m_pRecordSet;
}

DataWriter* DataWriter::clone()
{
    return new DataWriter( m_pDB );
}

void DataWriter::beginWriteData( const CString& tableName )
{
    CString sql = _T( "select * from " ) + tableName;
    m_pRecordSet->Open( dbOpenDynaset, sql, 0 );
    m_pRecordSet->AddNew();
}

long DataWriter::endWriteData()
{
    m_pRecordSet->Update();

    m_pRecordSet->SetBookmark( m_pRecordSet->GetLastModifiedBookmark() );
    COleVariant var;
    m_pRecordSet->GetFieldValue( _T( "编号" ), var );
    var.ChangeType( VT_I4 );

    m_pRecordSet->Close();
    return var.lVal; // long类型
}

void DataWriter::writeStringData( const CString& fieldName, const CString& value )
{
    if( value.GetLength() == 0 ) return;
    try
    {
        m_pRecordSet->SetFieldValue( fieldName, ( LPCTSTR )value );
    }
    catch ( CDaoException* e )
    {
        AfxMessageBox( e->m_pErrorInfo->m_strDescription );
        e->Delete();
    }
}

void DataWriter::writeIntData( const CString& fieldName, int value )
{
    // 超过整数范围，则不写入数据
    if( ( value == INT_MIN ) || ( value == INT_MAX ) ) return;

    CString strValue;
    strValue.Format( _T( "%d" ), value );
    writeStringData( fieldName, strValue );
}

void DataWriter::writeRealData( const CString& fieldName, double value )
{
    // 超过浮点数范围，则不写入数据
    if( ( value == DBL_MAX ) || ( value == DBL_MIN ) ) return;

    CString strValue;
    strValue.Format( _T( "%.7f" ), value );
    writeStringData( fieldName, strValue );
}

// 在access中：-1表示真，0表示假
static short Bool2Short( bool value )
{
    return value ? -1 : 0;
}

void DataWriter::writeBoolData( const CString& fieldName, bool value )
{
    try
    {
        COleVariant covFieldValue;
        covFieldValue.ChangeType( VT_BOOL );
        covFieldValue.boolVal = Bool2Short( value );
        m_pRecordSet->SetFieldValue( fieldName, covFieldValue );
    }
    catch( CDaoException* e )
    {
        AfxMessageBox( e->m_pErrorInfo->m_strDescription );
        e->Delete();
    }
}

CString DataWriter::getDBDirPath() const
{
    CString path = m_pDB->GetName();
    return path.Left( path.ReverseFind( _T( '\\' ) ) );
}