#pragma once

#include "../MineGE/DataLink.h"

// ͼԪ����
enum TEMP_TYPE
{
	TEMP_COMMON = 1,    // ��ͨ���(�ǽ��羮�������ҩ�⡢�����)--�������ܷϳ�
	TEMP_TWS    = 2,    // �ɾ�����
	TEMP_MR     = 3,    // ��������
	TEMP_MR2    = 4     // ��������(��ҩ�⡢�����) -- Ŀǰ��ʵ����û�п��Ƿ���,�������ܷϳ�����
};

// �¶�ģ��
class TempDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	TEMP_TYPE type;          // ����
	CString name;            // ����
	double t;                // �¶�	
	int cm;                  // ���¿��ƴ�ʩ
};
