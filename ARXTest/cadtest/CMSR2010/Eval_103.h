#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "AFCDataLink.h"

class EvalData_103 : public EvalData
{
public:
	EvalData_103();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;      // �÷�ص�����	
	double maxQ;       // ����������
	double q;          // ʵ�ʷ���
};

class EvalDataExtractor_103 : public EvalDataExtractor
{
public:
	EvalDataExtractor_103();
	~EvalDataExtractor_103();

	virtual bool hasNext();
	virtual void doExtract(EvalData* pEvalData);

private:
	void init();
	void clean();
	typedef std::deque<AFCDataLink*> DataSet;
	DataSet::iterator itr;
	DataSet ds;

	// �����ֶ�Ҫ��
	// "����Ϣ"-->xxx��-->"��˹�ȼ�"(�����б�)
	// �μ�GAS_LEVELö�ٵĶ���
	// Ĭ��"����˹"
	int gas_level;
};

class Eval_103 : public Eval
{
public:
	virtual bool doEval(EvalData* pEvalData);

private:
	bool eval_once(EvalData* pEvalData);
};

class EvalFactory_103 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
