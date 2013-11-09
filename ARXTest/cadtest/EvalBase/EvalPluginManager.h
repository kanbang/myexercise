#pragma once

namespace PJ
{
	class EvalMethodManager;
	class EvalPlugin;
	class ArxLoader;
	struct EvalPluginInfo;

	class EvalPluginManager
	{
	public:
		EvalPluginManager(EvalMethodManager* pEvalMethodManager);
		~EvalPluginManager(void);

		void load( const CString &arxFilePath, const CString& serviceName);
		void unload( const CString &arxFilePath );
		void getPluginInfo(const CString &arxFilePath, EvalPluginInfo& info);
		void getAllLoadedPluginPaths(AcStringArray& paths);

	private:
		struct PluginInfo
		{
			CString arxFilePath;
			ArxLoader* pARXLoader;
			EvalPlugin* pEvalPlugin;
		};

		void unload( PluginInfo &info );
		void unloadAllPlugins(); // ж�����в��

		// ����arxģ�飬������һ��ARXLoader����
		ArxLoader* loadARXModule(const CString &arxFilePath, const CString& serviceName);

		// �������۲��
		EvalPlugin* createEvalPlugin(ArxLoader* pARXLoader);

	private:
		EvalMethodManager* m_pEvalMethodManager;
		typedef AcArray<PluginInfo> Plugins;
		Plugins m_plugins;
	};
}