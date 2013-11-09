#pragma once

#include "dlimexp.h"

/* ǰ������ */
class MineGEDraw;
class MineGEPluginManager;
class MineGEDrawManager;

/* 
 * ���ӻ�ϵͳ��������һ��ȫ�ֿ��ӻ�ϵͳ����
 * ���õ���ģʽ(Singleton Pattern)ʵ��
 * ֻ��ͨ����̬����GetInstance()�õ�����ָ��
 */
class MINEGE_DRAW_DLLIMPEXP MineGEDrawSystem
{
public:
	/* ����ȫ�ֿ��ӻ�ϵͳ���� */
	static MineGEDrawSystem* CreateInstance();
	/* ��ȡȫ�ֿ��ӻ�ϵͳ���� */
	static MineGEDrawSystem* GetInstance();
	/* ����ȫ�ֿ��ӻ�ϵͳ���� */
	static void DestroyInstance();

	// �������
public:
	// ���ؿ��ӻ�Ч�����
	void loadPlugin(const CString &arxFilePath);

	// ж�ؿ��ӻ�Ч�����
	void unloadPlugin(const CString &arxFilePath);

	// ���ӻ�Ч������
public:
	// ��ȡͼԪ��Ĭ�Ͽ��ӻ�Ч��
	MineGEDraw* getDefaultGEDraw(const CString& geType);

	// ��ȡͼԪ��Ĭ�Ͽ��ӻ�Ч��
	CString getDefaultGEDrawName(const CString& geType);

	// ��ȡͼԪ��ָ�����ӻ�Ч���Ķ���ָ��
	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	// ����ͼԪ�ĵ�ǰ���ӻ�Ч��
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// ��ȡͼԪ�����п��ӻ�Ч��(���ڱ���)
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

private:
	/* ����һ��ȫ�ֵľ�̬����ָ�룬ָ����ӻ�ϵͳ���� */
	static MineGEDrawSystem* m_pMineGEService;

	/* 
	 * ���ù��캯������������
	 * ������Աֻ��ͨ����̬����������ȫ�ֿ��ӻ����� 
	 */
	MineGEDrawSystem(void);
	~MineGEDrawSystem(void);

private:
	MineGEPluginManager* m_pMineGEPluginManager;
	MineGEDrawManager* m_pMineGEDrawManager;
};
