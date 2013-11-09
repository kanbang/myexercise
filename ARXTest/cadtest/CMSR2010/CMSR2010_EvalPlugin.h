#pragma once

#include "../EvalBase/EvalPlugin.h"
using namespace PJ;

class CMSR2010_EvalPlugin : public EvalPlugin
{
public:
	CMSR2010_EvalPlugin();
	~CMSR2010_EvalPlugin(void);
	
	// 初始化
	virtual void initialize(EvalMethod* pEvalMethod);

	// 卸载清理
	virtual void unInitialize(EvalMethod* pEvalMethod);

	// 获取插件信息
	virtual void getPluginInfo(EvalPluginInfo& info) const;
};
