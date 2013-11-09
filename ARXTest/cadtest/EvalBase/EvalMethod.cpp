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
            // ����delete itr->second����ΪpEvalFactory���ڴ�����dll��ͨ����̬�����ķ��������
            // delete itr->second;
            // ��ԭ���ĸ��ǵ�
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