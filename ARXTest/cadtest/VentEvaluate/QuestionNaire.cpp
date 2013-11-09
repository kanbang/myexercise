#include "stdafx.h"
#include "DBFieldNames.h"
#include "QuestionNaireDlg.h"

#include "../EvalBase/EvalResult.h"
using namespace PJ;

// 获取没有实现评价的条款(评价结果=未知)
static void GetNoEvalClauseNums( const CString& evalResultDBPath,
                                 const CString& caclResultTable,
                                 AcDbIntArray& clauses )
{
    //CAcModuleResourceOverride myResources;

    CDaoDatabase db;
    try
    {
        db.Open( evalResultDBPath );

        CDaoRecordset rs( &db );
        CString sql;
        sql.Format( _T( "select * from %s where %s=%d" ), caclResultTable, EVAL_RESULT_FIELD, UnknownResult );
        rs.Open( dbOpenDynaset, sql, 0 );

        rs.MoveFirst();
        while( !rs.IsEOF() )
        {
            COleVariant v1, v2;
            rs.GetFieldValue( CLAUSE_NUM_FIELD , v1 );

            clauses.append( ( int )v1.lVal );

            rs.MoveNext () ;
        }

        rs.Close();
        db.Close();
    }
    catch( CDaoException* e )
    {
        e->ReportError();
        e->Delete();

        if( db.IsOpen() ) db.Close();
    }
}

static void ShowQuestionNaireDlg( const CString& regulateDB, const CString& regulationTable,
                                  const AcDbIntArray& clauses, AcDbIntArray& result )
{
    //CAcModuleResourceOverride myResources;

    QuestionNaireDlg qnd;

    // 输入数据
    qnd.m_regulateDB = regulateDB;
    qnd.m_regulationTable = regulationTable;

    int len = clauses.length();
    for( int i = 0; i < len; i++ )
    {
        qnd.addClause( clauses[i] );
    }
    // 显示调查对话框
    qnd.DoModal();

    // 效率较差，目前就这样了!!!
    // 返回结果，写入到数据库
    for( int i = 0; i < len; i++ )
    {
        result.append( qnd.getResult( clauses[i] ) );
    }
}

static int GetQuestionNaireResult( const AcDbIntArray& clauses, const AcDbIntArray& result, int clause )
{
    int qnr = -1;
    int len = clauses.length();
    for( int i = 0; i < len; i++ )
    {
        if( clauses[i] == clause )
        {
            qnr = result[i];
            break;
        }
    }
    return qnr;
}

static void WriteQuestionNaireResultToDB( const CString& evalResultDBPath,
        const CString& caclResultTable,
        const AcDbIntArray& clauses,
        const AcDbIntArray& result )
{
    //CAcModuleResourceOverride myResources;

    CDaoDatabase db;
    try
    {
        db.Open( evalResultDBPath );

        CDaoRecordset rs( &db );
        CString sql;
        sql.Format( _T( "select * from %s where %s=%d" ), caclResultTable, EVAL_RESULT_FIELD, UnknownResult );
        rs.Open( dbOpenDynaset, sql, 0 );
        rs.MoveFirst();

        while( !rs.IsEOF() )
        {
            COleVariant v1;
            rs.GetFieldValue( CLAUSE_NUM_FIELD, v1 );

            int qnr = GetQuestionNaireResult( clauses, result, v1.intVal );
            if( qnr != -1 )
            {
                rs.Edit();
                COleVariant v2;
                v2.ChangeType( VT_I4 ); // int
                v2.lVal = qnr;
                rs.SetFieldValue( EVAL_RESULT_FIELD, v2 );
                rs.Update();
            }
            rs.MoveNext();
        }

        rs.Close();
        db.Close();
    }
    catch ( CDaoException* e )
    {
        e->ReportError();
        e->Delete();

        if( db.IsOpen() ) db.Close();
    }
}

void QuestionNaire( const CString& regulateDB, const CString& regulationTable,
                    const CString& evalResultDBPath, const CString& caclResultTable )
{
    CAcModuleResourceOverride myResources;

    AcDbIntArray clauses;
    GetNoEvalClauseNums( evalResultDBPath, caclResultTable, clauses );
    if( clauses.isEmpty() ) return;

    AcDbIntArray result;
    ShowQuestionNaireDlg( regulateDB, regulationTable, clauses, result );

    WriteQuestionNaireResultToDB( evalResultDBPath, caclResultTable, clauses, result );
}
