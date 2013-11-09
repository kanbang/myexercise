#pragma once

#include "../MineGE/DataLink.h"

class WSDataLink : public DataLink
{
public:
	WSDataLink(void);
	~WSDataLink(void);

protected:
	virtual void regDatas();

public:
	CString name;   // ����������
	bool hasRisk;   // �Ƿ���ͻ��Σ��
	double iz;      // ����������
	double oz;      // �ط�������
	bool down;      // �Ƿ�����ͨ��
};
