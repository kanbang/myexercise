#pragma once

#include "../MineGEDraw/MineGEPlugin.h"

class DefGEPlugin : public MineGEPlugin
{
public:
	virtual void initialize(MineGEDrawManager* pGEDrawManager);

	// ж�ز��֮ǰ��������
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager);
};
