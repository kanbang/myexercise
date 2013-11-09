#pragma once

#include "MineGEDrawManager.h"
#include "HelperMacro.h"

class MineGEPlugin
{
public:
	virtual ~MineGEPlugin() {}

	// ���ز����ĳ�ʼ������
	virtual void initialize(MineGEDrawManager* pGEDrawManager) = 0;

	// ж�ز��֮ǰ��������
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager) = 0;	
};

typedef MineGEPlugin* (*MineGEPluginCreateFunc)();