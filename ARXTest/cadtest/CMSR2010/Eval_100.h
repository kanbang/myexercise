#pragma once

#include "../EvalBase/Eval.h"
using namespace PJ;

#include <deque>

// �ɾ���ģ��
#include "TWSDataLink.h"

class EvalData_100 : public EvalData
{
public:
	EvalData_100();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);

public:
	CString name;      // ����
	double O2;         // ����Ũ��
	double CO2;        // ������̼Ũ��
	double CO;         // һ����̼Ũ��
	double NO;         // һ������Ũ�ȣ���ú���ĵ�100�������漰����
	double SO2;        // ��������Ũ�ȣ�ͬ�ϣ�
	double H2S;        // ����Ũ�ȣ�ͬ�ϣ�
	double NH3;        // ��Ũ�ȣ�ͬ�ϣ�
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