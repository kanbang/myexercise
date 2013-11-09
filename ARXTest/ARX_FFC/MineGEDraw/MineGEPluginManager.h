#pragma once

#include <deque>

class MineGEPlugin;
class MineGEDrawManager;
class ArxLoader;

class MineGEPluginManager
{
public:
	MineGEPluginManager(MineGEDrawManager* pMineGEDrawManager);
	~MineGEPluginManager(void);
	
	void load(const CString &arxFilePath, const CString& serviceName);
	void unload(const CString &arxFilePath);

private:
	struct PluginInfo
	{
		CString pluginFilePath; // �������·��
		ArxLoader* pARXLoader;  // arxģ�������
		MineGEPlugin* pMineGEPlugin; // �������
	};

	void unload( PluginInfo &plugIn );

	void unloadAllPlugins(); // ж�����в��

	// ����arxģ�飬������һ��ARXLoader����
	ArxLoader* loadARXModule(const CString &arxFilePath, const CString& serviceName);

	// ����ͼԪ���
	MineGEPlugin* createMineGEPlugin(ArxLoader* pARXLoader);

private:
	MineGEDrawManager* m_pMineGEDrawManager;
	typedef std::deque<PluginInfo> Plugins;
	Plugins m_plugins;
};
