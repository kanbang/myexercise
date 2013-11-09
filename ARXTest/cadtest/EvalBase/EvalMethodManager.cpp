#include "StdAfx.h"
#include "EvalMethodManager.h"
#include "EvalMethod.h"

namespace PJ
{
    EvalMethodManager::EvalMethodManager( void )
    {
    }

    EvalMethodManager::~EvalMethodManager( void )
    {
    }

    void EvalMethodManager::regEvalMethod( const CString& regulationName )
    {
        EvalMethodCollection::iterator itr = ems.find( regulationName );
        if( itr != ems.end() ) return;

        ems.insert( EvalMethodCollection::value_type( regulationName, new EvalMethod( regulationName ) ) );
    }

    EvalMethod* EvalMethodManager::getEvalMethodByName( const CString& regulationName )
    {
        EvalMethodCollection::iterator itr = ems.find( regulationName );
        if( itr == ems.end() ) return 0;

        return itr->second;
    }
}