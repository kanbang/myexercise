#pragma once

#include "../EvalBase/Eval.h"
using namespace PJ;

// 计数器上限
// 用于DefEvalDataExtractor，表示只执行一次eval
#define COUNT_LIMIT 1

// 默认合格的空评价
class DefEvalData : public EvalData
{
protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
};

class DefEvalDataExtractor : public EvalDataExtractor
{
public:
	DefEvalDataExtractor();
	~DefEvalDataExtractor();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);

private:
	int count;     // 计数器，
};

class DefEval : public Eval
{
public:
	virtual EvalResult doEval(EvalData* pEvalData);
};

class DefEvalFactory : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
