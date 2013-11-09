#pragma once

#include "../EvalBase/Eval.h"
using namespace PJ;

#include <deque>

#include "TempDataLink.h"

class EvalData_102 : public EvalData
{
public:
	EvalData_102();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	TEMP_TYPE type; // 图元类型（井筒、工作面、机电硐室）

	// 通用数据
	CString name;      // 名称
	double t;          // 温度
	double minT;       // 最低温度
	double maxT;       // 最高温度
	int cm;            // 控制措施
	CString strCM;     // 控制措施(字符串)
};

class EvalDataExtractor_102 : public EvalDataExtractor
{
public:
	EvalDataExtractor_102();
	~EvalDataExtractor_102();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);

private:
	void init();
	void init1(); // 初始化井巷
	void init2(); // 初始化回采和掘进工作面
	void init3(); // 初始化机电硐室
	void init4(); // 初始化其它图元(火药库、充电室、风桥) -- 目前的实现中没有考虑风桥,后续可能废除风桥
	void clean(); // 清理测试集合
	typedef std::deque<TempDataLink*> DataSet;
	DataSet ds;
	DataSet::iterator itr;

	void initMinMaxTemperature1(EvalData_102* pVData);
	void initMinMaxTemperature2(EvalData_102* pVData);
	void initMinMaxTemperature3(EvalData_102* pVData);
	void initMinMaxTemperature4(EvalData_102* pVData);
};

class Eval_102 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_102 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};