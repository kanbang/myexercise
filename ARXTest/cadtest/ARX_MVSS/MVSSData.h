#pragma once

#include "../DaoHelper/DaoData2.h"

#define JOINT_TABLE_NAME _T("节点")
#define TUNNEL_TABLE_NAME _T("巷道")
#define GATE_TABLE_NAME _T("构筑物")
#define FAN_TABLE_NAME _T("通风动力")

#define INVALID_OBJ_ID -1

class MVSSDataLink;

// 需要同时具备读写功能
class MVSSData : public DaoData2
{
protected:
	MVSSData(const CString& tableName);

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int objId;  // 标识
};

// 节点数据
class JointData : public MVSSData
{
public:
	JointData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	double x, y, z;   // 几何坐标

	CString name;     // 名称
	CString rp;       // 相对压能
	CString p;        // 大气压
	CString comment;  // 备注
};

// 巷道数据
class TunnelData : public MVSSData
{
public:
	TunnelData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int spt;          // 始节点标识
	int ept;          // 末节点标识

	CString name;     // 名称
	CString q;        // 风量
	CString r;        // 摩擦风阻
	CString lr;       // 局部风阻
	CString h;        // 总阻力
	CString fq;       // 固定风量
	CString rho;      // 密度
	CString ad;       // 可调节性
	CString ah;       // 调节阻力
	CString ar;       // 调节风阻
	CString rho1;     // 春季密度
	CString rho2;     // 夏季密度
	CString rho3;     // 秋季密度
	CString rho4;     // 冬季密度
	CString tq;       // 测试风量
	CString th;       // 测试阻力
	CString l;        // 巷道长度
	CString apha;     // 摩擦阻力系数
	CString r100;     // 百米摩擦风阻
	CString atype;    // 断面形状
	CString area;     // 断面面积
	CString c;        // 周长
	CString st;       // 支护方式
	CString maxV;     // 最大允许风速
	CString tUse;     // 巷道用途
	CString tRegion;  // 三区
	CString temp;     // 温度
	CString humidity; // 湿度
	CString O2;       // 氧气浓度
	CString CH4;      // 甲烷浓度
	CString CO2;      // 二氧化碳浓度
	CString CO;       // 一氧化碳浓度
	CString reliability; // 可靠性
	CString type;     // 类型
	CString comment;  // 备注
	CString eh;       // 附加阻力
};

// 构筑物数据
class GateData : public MVSSData
{
public:
	GateData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int tObjId;         // 关联的巷道标识
	double x, y, z;   // 几何坐标

	CString name;     // 名称
	CString	rq;       // 压差
	CString flow;     // 漏风量
	CString er;       // 等效风阻
	CString ea;       // 等效风窗面积
	CString trq;      // 测试压差
};

// 风机数据
class FanData : public MVSSData
{
public:
	FanData();

protected:
	virtual void createTableImpl(TableCreator* pTableCreator);
	virtual void writeToTableImpl(DataWriter* pDataWriter);
	virtual void readFromTableImpl(DataReader* pDataReader);

public:
	int tObjId;         // 关联的巷道标识
	double x, y, z;   // 几何坐标

	CString name;  // 名称
	CString rs;    // 运行状态
	CString q;     // 风量
	CString p;     // 风压
	CString power; // 功率
	CString efficiency; // 效率
	CString r;     // 风阻
	CString aHole; // 等积孔
	CString model; // 型号
	CString angle; // 叶片角度
	CString	rotateSpeed; // 转速
	CString fq;    // 风机风量
	CString nq;    // 风网风量
	CString lq;    // 漏风风量
	CString fsp;   // 风机静压
	CString ffp;   // 风机全压
	CString outVP; // 出口速压
	CString inVP;  // 入口速压
	CString inSP;  // 入口静压
	CString tSP;   // 测试静压
	CString cPower;// 耗电功率
	CString inPower;// 输入功率
	CString outPower; // 输出功率
	CString nPower;   // 风网功率
	CString lPower;   // 漏风功率
	CString uPower;   // 有用效率
	CString ePower;   // 电机效率
	CString sPower;   // 静压效率
	CString fPower;   // 全压效率
	CString fr;       // 风机风阻
	CString nr;       // 风网风阻
	CString lr;       // 漏风风阻
	CString fma;      // 风硐断面
	CString fmv;      // 风硐风速
	CString outArea;  // 出口断面
	CString outV;     // 出口风速
	CString iq;       // 初始风量
	CString comment;  // 备注
	CString	H_a0;
	CString H_a1;
	CString	H_a2;
	CString	H_a3;
	CString	H_a4;
	CString	H_a5;
	CString	N_a0;
	CString	N_a1;
	CString	N_a2;
	CString	N_a3;
	CString	N_a4;
	CString	N_a5;
	CString	E_a0;
	CString	E_a1;
	CString	E_a2;
	CString	E_a3;
	CString	E_a4;
	CString	E_a5;
};