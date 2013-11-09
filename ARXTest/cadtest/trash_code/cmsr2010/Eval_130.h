#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "FipDataLink.h"

class EvalData_130 : public EvalData
{
public:
	EvalData_130();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;        // 名称
	AcStringArray p1;
	AcStringArray p2;
	bool includeAir;    // 是否包含用风地点(串联风)
};

class EvalDataExtractor_130 : public EvalDataExtractor
{
public:
	EvalDataExtractor_130();
	~EvalDataExtractor_130();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);	

private:
	void init();
	void clean();
	typedef std::deque<FipDataLink*> DataSet;
	DataSet::iterator itr;
	DataSet ds;
};

class Eval_130 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_130 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
