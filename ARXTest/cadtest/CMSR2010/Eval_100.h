#pragma once

#include "../EvalBase/Eval.h"
using namespace PJ;

#include <deque>

// 采掘面模型
#include "TWSDataLink.h"

class EvalData_100 : public EvalData
{
public:
	EvalData_100();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;      // 名称
	double O2;         // 氧气浓度
	double CO2;        // 二氧化碳浓度
	double CO;         // 一氧化碳浓度
	double NO;         // 一氧化氮浓度（在煤安的第100条中有涉及到）
	double SO2;        // 二氧化硫浓度（同上）
	double H2S;        // 硫化氢浓度（同上）
	double NH3;        // 氨浓度（同上）
};

class EvalDataExtractor_100 : public EvalDataExtractor
{
public:
	EvalDataExtractor_100();
	~EvalDataExtractor_100();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);

private:
	void init();
	void clean();
	typedef std::deque<TWSDataLink*> DataSet;
	DataSet ds;
	DataSet::iterator itr;
};

class Eval_100 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
	bool eval_O2(double O2);
	bool eval_CO2(double CO2);
	bool eval_CO(double CO);
	bool eval_NO(double NO);
	bool eval_SO2(double SO2);
	bool eval_H2S(double H2S);
	bool eval_NH3(double NH3);
};

class EvalFactory_100 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};