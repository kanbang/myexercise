#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "WSDataLink.h"

class EvalData_115 : public EvalData
{
public:
	EvalData_115();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;   // 工作面名称
	bool hasRisk;   // 是否有突出危险
	double iz;      // 进风巷道标高
	double oz;      // 回风巷道标高
	bool down;      // 是否下行通风
};

class EvalDataExtractor_115 : public EvalDataExtractor
{
public:
	EvalDataExtractor_115();
	~EvalDataExtractor_115();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);	

private:
	void init();
	void clean();
	typedef std::deque<WSDataLink*> DataSet;
	DataSet::iterator itr;
	DataSet ds;
};

class Eval_115 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_115 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
