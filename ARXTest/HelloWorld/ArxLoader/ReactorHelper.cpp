#include "StdAfx.h"
#include "ReactorHelper.h"

#include "CadQuitEditorReactor.h"
#include "DocReactor.h"
#include "DBReactor.h"

CadQuitEditorReactor* pCadQuitEditorReactor = 0;
DocReactor* pDocReactor = 0;
DBReactor* pDBReactor = 0;

void ReactorHelper::CreateCadQuitEditorReactor()
{
    if( pCadQuitEditorReactor == 0 )
    {
        pCadQuitEditorReactor = new CadQuitEditorReactor( true );
    }
}

void ReactorHelper::RemoveCadQuitEditorReactor()
{
    delete pCadQuitEditorReactor;
    pCadQuitEditorReactor = 0;
}

void ReactorHelper::CreateDocReactor()
{
    if( pDocReactor == 0 )
    {
        pDocReactor = new DocReactor( true );
    }
}

void ReactorHelper::RemoveDocReactor()
{
    delete pDocReactor;
    pDocReactor = 0;
}

void ReactorHelper::CreateDBReactor()
{
    if( pDBReactor == 0 )
    {
        pDBReactor = new DBReactor( true );
    }
}

void ReactorHelper::RemoveDBReactor()
{
    delete pDBReactor;
    pDBReactor = 0;
}