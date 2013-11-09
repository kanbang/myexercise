#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
#include "VelocityDataLink.h"

using namespace PJ;

class EvalData_101 : public EvalData
{
public:
	EvalData_101();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	VD_TYPE type; // 图元类型（巷道、井筒、工作面、掘进工作面等）

	// 井巷通用数据
	CString name;      // 井巷名称
	double q_ch4;       // 平均绝对瓦斯涌出量
	double v;          // 风速
	double minV;       // 最低风速
	double maxV;       // 最高风速	

	// 巷道
	int ft;             // 巷道功能类型
	int sp;             // 井筒附加属性(巷道中的"井筒"特有)
	bool isShaft;       // 是否井筒

	// 采掘面
	int tm;            // 材质类型(Tunnel Material)
	int cm;            // 采煤方法(回采工作面独有)cm

	// 条款101的字段--[表2  井巷中的允许风流速度]
	CString typeName;       // 井巷风速类型名称
};

class EvalDataExtractor_101 : public EvalDataExtractor
{
public:
	EvalDataExtractor_101();
	~EvalDataExtractor_101();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);

private:
	void init();
	void init1(); // 初始化回采工作面
	void init2(); // 初始化掘进工作面
	void init3(); // 初始化普通巷道
	void init4(); // 初始化风桥 -- 后续可能废除
	void clean(); // 清理测试集合
	typedef std::deque<VelocityDataLink*> DataSet;
	DataSet ds;
	DataSet::iterator itr;

	void initShaftVelocity(EvalData_101* pVData);
	void initMinMaxVelocity1(EvalData_101* pVData);
	void initMinMaxVelocity2(EvalData_101* pVData);
	void initMinMaxVelocity3(EvalData_101* pVData);
	void initMinMaxVelocity4(EvalData_101* pVData);
};

class Eval_101 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
	// ???浮点数的比较可能存在问题???
	bool eval_v(double v, double minV, double maxV);
};

class EvalFactory_101 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
