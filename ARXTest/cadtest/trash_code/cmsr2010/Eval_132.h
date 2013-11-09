#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "FipDataLink.h"

class EvalData_132 : public EvalData
{
public:
	EvalData_132();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;        // ����
	AcStringArray p1;
	AcStringArray p2;
	bool includeAir;    // �Ƿ�����÷�ص�(������)
};

class EvalDataExtractor_132 : public EvalDataExtractor
{
public:
	EvalDataExtractor_132();
	~EvalDataExtractor_132();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);	

private:
	void init();
	void clean();
	typedef std::deque<FipDataLink*> DataSet;
	DataSet::iterator itr;
	DataSet ds;
};

class Eval_132 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_132 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
