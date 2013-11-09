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
		void unloadAllPlugins(); // 卸载所有插件

		// 加载arx模块，并返回一个ARXLoader对象
		ArxLoader* loadARXModule(const CString &arxFilePath, const CString& serviceName);

		// 创建评价插件
		EvalPlugin* createEvalPlugin(ArxLoader* pARXLoader);

	private:
		EvalMethodManager* m_pEvalMethodManager;
		typedef AcArray<PluginInfo> Plugins;
		Plugins m_plugins;
	};
}