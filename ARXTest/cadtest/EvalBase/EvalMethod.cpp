#include "StdAfx.h"
#include "EvalMethod.h"
#include "Eval.h"

namespace PJ
{
    EvalMethod::EvalMethod( const CString& regulation ) : m_regulation( regulation )
    {
    }

    EvalMethod::~EvalMethod( void )
    {
    }

    CString EvalMethod::getRegulationName() const
    {
        return m_regulation;
    }

    void EvalMethod::regEvalFactory( int clauseNum, EvalFactory* pEvalFactory )
    {
        EvalFactoryCollection::iterator itr = factories.find( clauseNum );
        if( itr == factories.end() )
        {
            factories.insert( EvalFactoryCollection::value_type( clauseNum, pEvalFactory ) );
        }
        else
        {
            // 不能delete itr->second，因为pEvalFactory的内存是在dll中通过静态变量的方法分配的
            // delete itr->second;
            // 将原来的覆盖掉
            itr->second = pEvalFactory;
        }
    }

    void EvalMethod::unRegEvalFactory( int clauseNum )
    {
        EvalFactoryCollection::iterator itr = factories.find( clauseNum );
        if( itr == factories.end() ) return;

        factories.erase( itr );
    }

    EvalFactory* EvalMethod::getEvalFactoryByClauseNum( int clauseNum )
    {
        EvalFactoryCollection::iterator itr = factories.find( clauseNum );
        if( itr == factories.end() ) return 0;

        return itr->second;
    }
}