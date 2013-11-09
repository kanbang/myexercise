#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "ILTDataLink.h"

// 简化操作
// 只判断联络巷上是否有构筑物
// 包括风门、风窗、挡风墙
// 不完全遵守第109条的要求
class EvalData_109 : public EvalData
{
public:
	EvalData_109();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;        // 联络巷名称
	CString inEdgeName;  // 主进风巷
	CString outEdgeName; // 主回风巷
	bool hasWall;        // 是否有构筑物
};

class EvalDataExtractor_109 : public EvalDataExtractor
{
public:
	EvalDataExtractor_109();
	~EvalDataExtractor_109();
	
	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);	

private:
	void init();
	void clean();
	typedef std::deque<ILTDataLink*> DataSet;
	DataSet::iterator itr;
	DataSet ds;
};

class Eval_109 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_109 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
