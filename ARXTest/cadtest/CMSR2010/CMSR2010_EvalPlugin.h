#pragma once

#include "../EvalBase/EvalPlugin.h"
using namespace PJ;

class CMSR2010_EvalPlugin : public EvalPlugin
{
public:
	CMSR2010_EvalPlugin();
	~CMSR2010_EvalPlugin(void);
	
	// ��ʼ��
	virtual void initialize(EvalMethod* pEvalMethod);

	// ж������
	virtual void unInitialize(EvalMethod* pEvalMethod);

	// ��ȡ�����Ϣ
	virtual void getPluginInfo(EvalPluginInfo& info) const;
};
