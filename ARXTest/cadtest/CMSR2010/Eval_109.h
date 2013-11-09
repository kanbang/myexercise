#pragma once

#include <deque>
#include "../EvalBase/Eval.h"
using namespace PJ;

#include "ILTDataLink.h"

// �򻯲���
// ֻ�ж����������Ƿ��й�����
// �������š��細������ǽ
// ����ȫ���ص�109����Ҫ��
class EvalData_109 : public EvalData
{
public:
	EvalData_109();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;        // ����������
	CString inEdgeName;  // ��������
	CString outEdgeName; // ���ط���
	bool hasWall;        // �Ƿ��й�����
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
