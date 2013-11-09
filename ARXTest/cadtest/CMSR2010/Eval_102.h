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
	TEMP_TYPE type; // ͼԪ���ͣ���Ͳ�������桢�������ң�

	// ͨ������
	CString name;      // ����
	double t;          // �¶�
	double minT;       // ����¶�
	double maxT;       // ����¶�
	int cm;            // ���ƴ�ʩ
	CString strCM;     // ���ƴ�ʩ(�ַ���)
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
	void init1(); // ��ʼ������
	void init2(); // ��ʼ���زɺ;��������
	void init3(); // ��ʼ����������
	void init4(); // ��ʼ������ͼԪ(��ҩ�⡢����ҡ�����) -- Ŀǰ��ʵ����û�п��Ƿ���,�������ܷϳ�����
	void clean(); // ������Լ���
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