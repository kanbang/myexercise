#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "StationDataLink.h"

class EvalData_105 : public EvalData
{
public:
	EvalData_105();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;        // 应测风地点名称	
	bool hasWindStation; // 是否有测风站
	bool hasDone;        // 是否按规定测风
};

class EvalDataExtractor_105 : public EvalDataExtractor
{
public:
	EvalDataExtractor_105();
	~EvalDataExtractor_105();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);	

private:
	void init();
	void clean();
	typedef std::deque<StationTunnelDataLink*> DataSet;
	DataSet::iterator itr;
	DataSet ds;
};

class Eval_105 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_105 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
