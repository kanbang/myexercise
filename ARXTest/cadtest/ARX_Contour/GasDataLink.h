#pragma once

#include "../MineGE/DataLink.h"

class GasDataLink : public DataLink
{
protected:
	virtual void regDatas()
	{
		linkDoubleData(name, &z);
	}

public:
	double z;     // 瓦斯点z值(瓦斯含量、瓦斯压力、瓦斯涌出量)
	CString name; // 要读取的字段名称(外部设置!!!)
};
