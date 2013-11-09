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
    pEvalData->simpleCause = _T( "�����������" );

    return UNKNOWN_RESULT; // ���۽��: "δ֪"
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
