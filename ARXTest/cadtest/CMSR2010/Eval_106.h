#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "InstrumentDataLink.h"

class EvalData_106 : public EvalData
{
public:
	EvalData_106();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;        // �Ǳ�����
	CString mn;          // �ͺ�(Model Number)
	int amount;          // �Ǳ�����
	bool qi;             // �Ƿ�ͨ����������(Quantitative Inspection)
};

class EvalDataExtractor_106 : public EvalDataExtractor
{
public:
	EvalDataExtractor_106();
	~EvalDataExtractor_106();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);	

private:
	void init();
	void clean();
	typedef std::deque<InstrumentDataLink*> DataSet;
	DataSet::iterator itr;
	DataSet ds;
};

class Eval_106 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_106 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
