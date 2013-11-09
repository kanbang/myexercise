#include "StdAfx.h"
#include "DefEval.h"

void DefEvalData::createTableImpl( TableCreator* pTableCreator )
{
    EvalData::createTableImpl( pTableCreator );
}

void DefEvalData::writeToTableImpl( DataWriter* pDataWriter )
{
    EvalData::writeToTableImpl( pDataWriter );
}

DefEvalDataExtractor::DefEvalDataExtractor() : count( 0 )
{

}

DefEvalDataExtractor::~DefEvalDataExtractor()
{

}

bool DefEvalDataExtractor::hasNext()
{
    return count < COUNT_LIMIT;
}

void DefEvalDataExtractor::doExtract( EvalData* pEvalData )
{
    count++;
}

EvalResult DefEval::doEval( EvalData* pEvalData )
{
    pEvalData->simpleCause = _T( "管理类的条款" );

    return UNKNOWN_RESULT; // 评价结果: "未知"
}

Eval* DefEvalFactory::createEvalObject()
{
    return new DefEval();
}

EvalData* DefEvalFactory::createEvalDataObject()
{
    return new DefEvalData();
}

EvalDataExtractor* DefEvalFactory::createEvalDataExtractorObject()
{
    return new DefEvalDataExtractor();
}
