#pragma once

#include "MineGEDrawManager.h"

// ������һЩע��/��ע����ӻ�Ч���ĸ�����
// ���㿪����ʹ�ô�������/��
#include "HelperMacro.h"

class MineGEPlugin
{
public:
	virtual ~MineGEPlugin() {}

	// ���ز����ĳ�ʼ������
	// ע�����а��������п��ӻ�Ч��
	virtual void initialize(MineGEDrawManager* pGEDrawManager) = 0;

	// ж�ز��֮ǰ��������
	// ��ע�����а��������п��ӻ�Ч��
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager) = 0;	
};

// ��������ָ��(�������������ǩ��)
typedef MineGEPlugin* (*MineGEPluginCreateFunc)();