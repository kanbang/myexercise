#include "StdAfx.h"
#include "TableCreator.h"

TableCreator::TableCreator( CDaoDatabase* pDB ) : m_pDB( pDB )
{
    m_pTableDef = new CDaoTableDef( pDB );
}

TableCreator::~TableCreator( void )
{
    delete m_pTableDef;
}

TableCreator* TableCreator::clone()
{
    return new TableCreator( m_pDB );
}

bool TableCreator::isTableExist( const CString& tableName )
{
    bool flag = false;

    // 方法1
    short count = m_pDB->GetTableDefCount();
    for( short i = 0; i < count; i++ )
    {
        CDaoTableDefInfo tbinfo;
        m_pDB->GetTableDefInfo( i, tbinfo );
        if( tbinfo.m_strName == tableName )
        {
            flag = true;
            break;
        }
    }

    /*
     *方法2
     * 该方法在GetTableDefInfo出抛出异常, 但不能被try-catch捕获
     * 有点莫名其妙，在其它普通C++项目中能够正常运行
     * 但在arx项目中却无法运行???
     * 猜测原因：debgu版本的crt是MD???
     */
    /*
    try
    {
    	CDaoTableDefInfo TableDefInfo;
    	m_pDB->GetTableDefInfo( tableName, TableDefInfo, AFX_DAO_PRIMARY_INFO );
    }
    catch( CDaoException* e)
    {
    	if( e->m_scode == E_DAO_VtoNameNotFound)
    	{
    		// Table does not exist
    		flag = false;
    	}
    	else
    	{
    		// Other errors
    		e->ReportError();
    		// ...
    		flag = false;
    	}
    	e->Delete();
    }
    */

    return flag;
}

void TableCreator::createTable( const CString& tableName )
{
    m_pTableDef->Create( tableName );
}

void TableCreator::closeTable()
{
    m_pTableDef->Append();
    m_pTableDef->Close();
}

// DAO创建主键的方法
// 参考：http://www.codeguru.com/forum/showthread.php?t=70689
//        http://www.codeguru.com/forum/showthread.php?t=247517
void TableCreator::addPrimaryKeyField( const CString& primarykeyName )
{
    m_pTableDef->CreateField( primarykeyName, dbLong, 4, dbAutoIncrField );

    CDaoIndexFieldInfo* pFieldInfo = new CDaoIndexFieldInfo;
    pFieldInfo->m_strName = primarykeyName;
    pFieldInfo->m_bDescending = FALSE;

    CDaoIndexInfo indexInfo;

    indexInfo.m_bPrimary = TRUE;
    indexInfo.m_bUnique = TRUE;
    indexInfo.m_bClustered = FALSE;
    indexInfo.m_bIgnoreNulls = FALSE;
    indexInfo.m_bRequired = FALSE;
    indexInfo.m_bForeign = FALSE;
    indexInfo.m_lDistinctCount = 0;
    indexInfo.m_strName = _T( "PrimaryKey" );
    indexInfo.m_pFieldInfos = pFieldInfo;
    indexInfo.m_nFields = 1;

    m_pTableDef->CreateIndex( indexInfo );

    delete pFieldInfo;
}

void TableCreator::addIntField( const CString& fieldName )
{
    m_pTableDef->CreateField( fieldName, dbLong, 0, dbFixedField | dbUpdatableField );
}

void TableCreator::addRealField( const CString& fieldName )
{
    m_pTableDef->CreateField( fieldName, dbDouble, 0, dbFixedField | dbUpdatableField );
}

void TableCreator::addBoolField( const CString& fieldName )
{
    m_pTableDef->CreateField( fieldName, dbBoolean, 0, dbFixedField | dbUpdatableField );
}

void TableCreator::addStringField( const CString& fieldName )
{
    m_pTableDef->CreateField( fieldName, dbText, 20, dbVariableField | dbUpdatableField );
}

void TableCreator::addCommentField( const CString& fieldName )
{
    m_pTableDef->CreateField( fieldName, dbMemo, 0, dbVariableField | dbUpdatableField );
}

CString TableCreator::getDBDirPath() const
{
    CString path = m_pDB->GetName();
    return path.Left( path.ReverseFind( _T( '\\' ) ) );
}