#pragma once

#include "../MineGEDraw/MineGEPlugin.h"

class FFC_GEPlugin : public MineGEPlugin
{
public:
	// ���ز���ĳ�ʼ������
	virtual void initialize(MineGEDrawManager* pGEDrawManager);

	// ж�ز��֮ǰ��������
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager);
};
