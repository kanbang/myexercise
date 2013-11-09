#pragma once

#include "TunnelNameDataLink.h"

class ILTDataLink : public TunnelNameDataLink
{
public:
	ILTDataLink(void);
	~ILTDataLink(void);

public:
	CString inEdgeName;  // ��������
	CString outEdgeName; // ���ط���
	bool hasWall;        // �Ƿ��й�����
};
