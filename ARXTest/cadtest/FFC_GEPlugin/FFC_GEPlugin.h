#pragma once

#include "../MineGEDraw/MineGEPlugin.h"

class FFC_GEPlugin : public MineGEPlugin
{
public:
	// 加载插件的初始化工作
	virtual void initialize(MineGEDrawManager* pGEDrawManager);

	// 卸载插件之前的清理工作
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager);
};
