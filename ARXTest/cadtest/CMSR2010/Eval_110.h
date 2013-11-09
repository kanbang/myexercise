#pragma once

#include "../EvalBase/Eval.h"
using namespace PJ;

#include <deque>

#include "LiftShaftDataLink.h"

class EvalData_110 : public EvalData
{
public:
	EvalData_110();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;    // ����
	double  v;       // ����
	int sp;          // ��Ͳ��������

	int vt;          // ͨ������(���ֶ�)
	bool hasLift;    // ���������豸
};

class EvalDataExtractor_110 : public EvalDataExtractor
{
public:
	EvalDataExtractor_110();
	~EvalDataExtractor_110();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);

private:
	void init();
	void clean();
	typedef std::deque<LiftShaftDataLink*> DataSet;
	DataSet ds;
	DataSet::iterator itr;
};

class Eval_110 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
	bool eval_ventInShaft(EvalData_110* pLSData); // ���۽��羮
	bool eval_ventOutShat(EvalData_110* pLSData); // ���ۻط羮
};

class EvalFactory_110 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};