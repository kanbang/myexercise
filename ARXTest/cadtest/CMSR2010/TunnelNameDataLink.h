#pragma once

#include "../MineGE/DataLink.h"

// ��������������
class TunnelNameDataLink : public DataLink
{
public:
	TunnelNameDataLink(void);
	~TunnelNameDataLink(void);

protected:
	virtual void regDatas();

public:
	CString name;
};
