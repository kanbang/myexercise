#pragma once

#include "../MineGE/DataLink.h"

// �ɾ�����ģ��--ú����100��
class TWSDataLink : public DataLink
{
protected:
	virtual void regDatas();

	// Ϊ�˼���������е������ֶζ�Ϊpublic���ͣ����ڷ���
public:
	CString name;      // ����
	double O2;         // ����Ũ��
	double CO2;        // ������̼Ũ��
	double CO;         // һ����̼Ũ��
	double NO;         // ������Ũ�ȣ���ú���ĵ�100�������漰����
	double SO2;        // ��������Ũ�ȣ�ͬ�ϣ�
	double H2S;        // ����Ũ�ȣ�ͬ�ϣ�
	double NH3;        // ��Ũ�ȣ�ͬ�ϣ�
};
