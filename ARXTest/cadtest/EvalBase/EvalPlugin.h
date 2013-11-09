#pragma once

#include "HelperMacro.h"
#include "EvalMethod.h"
#include "EvalPluginInfo.h"

namespace PJ
{
	// ���۲���ӿ�
	class EvalPlugin
	{
	public:
		virtual ~EvalPlugin() {}

		// ��ʼ��
		virtual void initialize(EvalMethod* pEvalMethod) = 0;

		// ж������
		virtual void unInitialize(EvalMethod* pEvalMethod) = 0;

		// ��ȡ�����Ϣ
		virtual void getPluginInfo(EvalPluginInfo& info) const = 0;
	};

	// ���۲����������ǩ��
	typedef EvalPlugin* (*EvalPluginCreateFunc)();
}