#pragma once

#include "MineGEDrawManager.h"

// 定义了一些注册/反注册可视化效果的辅助宏
// 方便开发，使得代码清晰/简单
#include "HelperMacro.h"

class MineGEPlugin
{
public:
	virtual ~MineGEPlugin() {}

	// 加载插件后的初始化工作
	// 注册插件中包含的所有可视化效果
	virtual void initialize(MineGEDrawManager* pGEDrawManager) = 0;

	// 卸载插件之前的清理工作
	// 反注册插件中包含的所有可视化效果
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager) = 0;	
};

// 声明函数指针(创建插件对象函数签名)
typedef MineGEPlugin* (*MineGEPluginCreateFunc)();