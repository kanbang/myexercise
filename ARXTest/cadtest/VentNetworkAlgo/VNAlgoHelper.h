#pragma once

#include "dlimexp.h"

// ͨ�������㷨������
class VNALGO_EXPORT_API VNAlgoHelper
{
public:
	// ִ��һ��ͨ���������
	static bool DoVNC(double Q, double precise, int maxCount);
	// ���ַ�����������(�����ط�)
	static bool DoSWD();
};

// ������඼�ǰ�װ��
// ������ѡͼԪ����Ҫ����·��
class FindIndependentPath;
class VNALGO_EXPORT_API FIPHelper
{
public:
	FIPHelper();
	~FIPHelper();
	void findAirEdges(AcDbObjectIdArray& objIds);
	bool doFIP(const AcDbObjectId& objId, AcDbObjectIdArray& p1, AcDbObjectIdArray& p2);
	void findSerialEdges(AcDbObjectIdArray& p, AcDbObjectIdArray& se);
	bool isIncludeAirEdge(const AcDbObjectIdArray& p);

private:
	FindIndependentPath* pFIP;
};

// ʶ��������
class IdentifyLiaisonTunnel;
class VNALGO_EXPORT_API ILTHelper
{
public:
	ILTHelper();
	~ILTHelper();

	bool doILT(AcDbObjectIdArray& objIds);

	// ʶ��������(�㷨2)
	bool doILT2(AcDbObjectIdArray& objIds);
	// ʶ��������(�㷨3)
	bool doILT3(AcDbObjectIdArray& objIds);
	// ʶ�����������ߺͳ��߷�֧����
	bool findInOutEdges(const AcDbObjectId& objId, AcDbObjectIdArray& s1, AcDbObjectIdArray& s2);

private:
	IdentifyLiaisonTunnel* pILT;
};


// ��ͨ���ж�
class VNALGO_EXPORT_API ConnectivityHelper
{
public:
	static bool Connected();                                         // �ж������Ƿ���ͨ��
	static bool DAG();                                               // �ж��������Ƿ���ڵ����·
	/* 
	 * ������ͨ��
	 * ccIds�ĳ��ȱ�ʾ��ͨ��ĸ���������ֵ������ͨ���ʶ
	 * ccIds[i]��ʾ��i����ͨ��ķ�֧����
	 * objIds�洢���е���ͨ��ķ�֧����(���ܴ��ڷ�֧�ظ�)
	 * ��i����ͨ���Ӧ�ķ�֧����ͨ��i��ccIds[i]����õ����䷶Χ
	 */
	static bool CC(AcDbObjectIdArray& objIds, AcDbIntArray& ccIds);   // ������ͨ��
	/*
	 * ͬ��
	 */
	static bool SCC(AcDbObjectIdArray& objIds, AcDbIntArray& sccIds); // ���ҵ����·
	static void FIE(AcDbObjectIdArray& objIds);                       // ���ҽ��羮
	static void FOE(AcDbObjectIdArray& objIds);                       // ���һط羮
	static void FIOE(AcDbObjectIdArray& objIds1, AcDbObjectIdArray& objIds2); // ���ҽ��ط羮
};

// ���Ҳ��վ
class FindWindStation;
class VNALGO_EXPORT_API FWSHelper
{
public:
	FWSHelper();
	~FWSHelper();

	AcDbObjectId doFWS(const AcDbObjectId& objId);

private:
	FindWindStation* pFWS;
};
