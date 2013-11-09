#pragma once

#include "dlimexp.h"

class MineGEDraw;
class MineGEDraw_Jig;
class MineGEPluginManager;
class MineGEDrawManager;

class MINEGE_DRAW_DLLIMPEXP MineGEDrawSystem
{
public:
	static MineGEDrawSystem* CreateInstance();
	static MineGEDrawSystem* GetInstance();
	static void DestroyInstance();

	// �������
public:
	// ���ػ���Ч�����
	void loadPlugin(const CString &arxFilePath);

	// ж�ػ���Ч�����
	void unloadPlugin(const CString &arxFilePath);

	// ����Ч������
public:
	// ��ȡͼԪ���͵�Ĭ�ϻ���Ч��
	MineGEDraw* getDefaultGEDraw(const CString& geType);

	// ��ȡͼԪ���͵�Ĭ�ϻ���Ч��
	CString getDefaultGEDrawName(const CString& geType);

	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	MineGEDraw_Jig* getGEDraw_Jig(const CString& geType, const CString& realGEDrawName);

	// ָ��ͼԪ���͵ĵ�ǰ����Ч��
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// ��ȡָ��ͼԪ���͵����л���Ч�������ڱ�����
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

private:
	static MineGEDrawSystem* m_pMineGEService;
	MineGEDrawSystem(void);
	~MineGEDrawSystem(void);

private:
	MineGEPluginManager* m_pMineGEPluginManager;
	MineGEDrawManager* m_pMineGEDrawManager;
};
