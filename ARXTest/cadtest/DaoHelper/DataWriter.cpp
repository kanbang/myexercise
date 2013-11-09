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
    m_pRecordSet->GetFieldValue( _T( "���" ), var );
    var.ChangeType( VT_I4 );

    m_pRecordSet->Close();
    return var.lVal; // long����
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
    // ����������Χ����д������
    if( ( value == INT_MIN ) || ( value == INT_MAX ) ) return;

    CString strValue;
    strValue.Format( _T( "%d" ), value );
    writeStringData( fieldName, strValue );
}

void DataWriter::writeRealData( const CString& fieldName, double value )
{
    // ������������Χ����д������
    if( ( value == DBL_MAX ) || ( value == DBL_MIN ) ) return;

    CString strValue;
    strValue.Format( _T( "%.7f" ), value );
    writeStringData( fieldName, strValue );
}

// ��access�У�-1��ʾ�棬0��ʾ��
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