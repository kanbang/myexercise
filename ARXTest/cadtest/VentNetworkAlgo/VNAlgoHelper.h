#pragma once

#include "dlimexp.h"

// 通风网络算法辅助类
class VNALGO_EXPORT_API VNAlgoHelper
{
public:
	// 执行一次通风网络解算
	static bool DoVNC(double Q, double precise, int maxCount);
	// 划分风流方向类型(进风或回风)
	static bool DoSWD();
};

// 下面的类都是包装类
// 搜索所选图元的主要进回路线
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

// 识别联络巷
class IdentifyLiaisonTunnel;
class VNALGO_EXPORT_API ILTHelper
{
public:
	ILTHelper();
	~ILTHelper();

	bool doILT(AcDbObjectIdArray& objIds);

	// 识别联络巷(算法2)
	bool doILT2(AcDbObjectIdArray& objIds);
	// 识别联络巷(算法3)
	bool doILT3(AcDbObjectIdArray& objIds);
	// 识别联络巷的入边和出边分支集合
	bool findInOutEdges(const AcDbObjectId& objId, AcDbObjectIdArray& s1, AcDbObjectIdArray& s2);

private:
	IdentifyLiaisonTunnel* pILT;
};


// 连通性判定
class VNALGO_EXPORT_API ConnectivityHelper
{
public:
	static bool Connected();                                         // 判断网络是否连通的
	static bool DAG();                                               // 判断网络中是否存在单向回路
	/* 
	 * 查找连通块
	 * ccIds的长度表示连通块的个数，索引值代表连通块标识
	 * ccIds[i]表示第i个连通块的分支个数
	 * objIds存储所有的连通块的分支集合(可能存在分支重复)
	 * 第i个连通块对应的分支可以通过i与ccIds[i]计算得到区间范围
	 */
	static bool CC(AcDbObjectIdArray& objIds, AcDbIntArray& ccIds);   // 查找连通块
	/*
	 * 同上
	 */
	static bool SCC(AcDbObjectIdArray& objIds, AcDbIntArray& sccIds); // 查找单向回路
	static void FIE(AcDbObjectIdArray& objIds);                       // 查找进风井
	static void FOE(AcDbObjectIdArray& objIds);                       // 查找回风井
	static void FIOE(AcDbObjectIdArray& objIds1, AcDbObjectIdArray& objIds2); // 查找进回风井
};

// 查找测风站
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
