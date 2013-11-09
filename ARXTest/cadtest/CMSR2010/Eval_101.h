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
	VD_TYPE type; // ͼԪ���ͣ��������Ͳ�������桢���������ȣ�

	// ����ͨ������
	CString name;      // ��������
	double q_ch4;       // ƽ��������˹ӿ����
	double v;          // ����
	double minV;       // ��ͷ���
	double maxV;       // ��߷���	

	// ���
	int ft;             // �����������
	int sp;             // ��Ͳ��������(����е�"��Ͳ"����)
	bool isShaft;       // �Ƿ�Ͳ

	// �ɾ���
	int tm;            // ��������(Tunnel Material)
	int cm;            // ��ú����(�زɹ��������)cm

	// ����101���ֶ�--[��2  �����е���������ٶ�]
	CString typeName;       // ���������������
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
	void init1(); // ��ʼ���زɹ�����
	void init2(); // ��ʼ�����������
	void init3(); // ��ʼ����ͨ���
	void init4(); // ��ʼ������ -- �������ܷϳ�
	void clean(); // ������Լ���
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
	// ???�������ıȽϿ��ܴ�������???
	bool eval_v(double v, double minV, double maxV);
};

class EvalFactory_101 : public EvalFactory
{
public:
	virtual Eval* createEvalObject();
	virtual EvalData* createEvalDataObject();
	virtual EvalDataExtractor* createEvalDataExtractorObject();
};
