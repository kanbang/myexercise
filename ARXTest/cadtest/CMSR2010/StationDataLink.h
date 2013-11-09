#pragma once

#include "../MineGE/DataLink.h"

// ��105������ʹ��
// Ӧ���ص������
class StationTunnelDataLink : public DataLink
{
public:
	StationTunnelDataLink();

protected:
	virtual void regDatas();

public:
	CString name;        // Ӧ���ص�����
	bool hasWindStation;
	bool hasDone;
};

// ���վͼԪ����
class StationDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	bool hasDone;  // �Ƿ񰴹涨���
};
