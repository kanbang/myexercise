#pragma once

#include "../EvalBase/Eval.h"
using namespace PJ;

// ����������
// ����DefEvalDataExtractor����ʾִֻ��һ��eval
#define COUNT_LIMIT 1

// Ĭ�Ϻϸ�Ŀ�����
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
	int count;     // ��������
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
