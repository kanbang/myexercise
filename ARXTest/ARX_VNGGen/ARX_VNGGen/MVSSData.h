#pragma once

// 无效的id
#define  INVALID_OBJ_ID -1

enum MVSS_DATA_TYPE
{
	MDT_NODE   = 0,
	MDT_EDGE  = 1,
	MDT_GATE    = 2,
	MDT_FAN     = 3
};

class MVSSData
{
public:
	MVSSData();
	virtual void readData(const CString& data) = 0;
	virtual bool isValid() const;

	int id;            // 对象标识
};

// 节点数据
class JointData : public MVSSData
{
public:
	JointData();
	virtual void readData(const CString& data);
};

// 巷道数据
class TunnelData : public MVSSData
{
public:
	TunnelData();
	virtual void readData(const CString& data);	
	virtual bool isValid() const;

	int spt;          // 始节点
	int ept;          // 末节点
};

// 构筑物数据
class GateData : public MVSSData
{
public:
	GateData();	
	virtual void readData(const CString& data);	
	virtual bool isValid() const;

	int tObjId;         // 关联的巷道标识
};

// 风机数据(通风动力)
class FanData : public MVSSData
{
public:
	FanData();	
	virtual void readData(const CString& data);
	virtual bool isValid() const;

	int tObjId;         // 关联的巷道标识
};

extern bool ReadComment(CDaoRecordset& rs, CString& data);
extern MVSSData* CreateMVSSData(MVSS_DATA_TYPE type);
extern CString GetTableName(MVSS_DATA_TYPE type);
extern CString GetTypeName(MVSS_DATA_TYPE type);
