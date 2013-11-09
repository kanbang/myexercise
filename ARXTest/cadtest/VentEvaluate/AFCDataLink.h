#pragma once

#include "../MineGE/DataLink.h"

// 工作面模型
class WS_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // 名称
	double area;                  // 断面面积
	double max_kdj, min_kdj;	  // 最大控顶距和最小控顶距
	double height;                // 实际采高
	double reasonalVelocity;      // 适宜风速
	double k_height;              // 采高调整系数
	double k_length;              // 长度调整系数
	double k_temperature;         // 温度调整系数

	double q_ch4;                 // 平均绝对瓦斯涌出量
	double k_ch4;                 // 瓦斯涌出不均衡通风系数
	double q_ch4_tail;            // 专排瓦斯巷瓦斯涌出量

	int maxPeopleNum;             // 工作面最多人数
	double maxWeight;             // 一次爆破炸药最大用量
};

// 掘进巷道模型
class TWS_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // 名称
	double area;                  // 断面面积
	int tunnelMaterial;           // 材质类型

	double q_ch4;                 // 平均绝对瓦斯涌出量
	double k_ch4;                 // 瓦斯涌出不均衡通风系数

	int fanNum;                   // 局部通风机台数
	double q_fan;                 // 局部通风机实际吸风量

	int maxPeopleNum;             // 工作面最多人数
	double maxWeight;             // 一次爆破炸药最大用量
};

// 爆炸材料库模型
class Powder_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // 名称
	double V;                     // 体积
};

// 充电室模型
class Charge_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // 名称
	double q_h2;                  // 氢气量
};

// 机电硐室模型
class Machine_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                // 名称
	
	double power;                // 功率
	double theta;                // 发热系数
	double delta_t;              // 温度差
};

enum AFC_NUM
{
	AN_1 = 0x0001,  // 其它巷道瓦斯涌出
	AN_2 = 0x0002,  // 风速验算
	AN_3 = 0x0004,  // 架线电车巷
};

// 胶轮车模型
class RTC_AFCDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	int num;                      // 台数
	double power;                 // 功率
};

// 其它巷道
class Tunnel_AFCDataLink : public DataLink
{
public:
	Tunnel_AFCDataLink();
	~Tunnel_AFCDataLink();

protected:
	virtual void regDatas();

public:
	bool needAir;                 //是否单独配风(如果为true,才需要进行计算)
	AFC_NUM an;                   // 进行哪些风量的计算

	CString name;                 // 名称
	double area;                  // 断面面积
	int tunnelMaterial;           // 材质类型
	double q_ch4;                 // 平均绝对瓦斯涌出量
	
	RTC_AFCDataLink* pRTCModel;
};
