#pragma once

// ��Ч��id
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

	int id;            // �����ʶ
};

// �ڵ�����
class JointData : public MVSSData
{
public:
	JointData();
	virtual void readData(const CString& data);
};

// �������
class TunnelData : public MVSSData
{
public:
	TunnelData();
	virtual void readData(const CString& data);	
	virtual bool isValid() const;

	int spt;          // ʼ�ڵ�
	int ept;          // ĩ�ڵ�
};

// ����������
class GateData : public MVSSData
{
public:
	GateData();	
	virtual void readData(const CString& data);	
	virtual bool isValid() const;

	int tObjId;         // �����������ʶ
};

// �������(ͨ�綯��)
class FanData : public MVSSData
{
public:
	FanData();	
	virtual void readData(const CString& data);
	virtual bool isValid() const;

	int tObjId;         // �����������ʶ
};

extern bool ReadComment(CDaoRecordset& rs, CString& data);
extern MVSSData* CreateMVSSData(MVSS_DATA_TYPE type);
extern CString GetTableName(MVSS_DATA_TYPE type);
extern CString GetTypeName(MVSS_DATA_TYPE type);
