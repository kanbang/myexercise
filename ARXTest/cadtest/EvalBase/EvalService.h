#pragma once

#include "dlimexp.h"

namespace PJ
{
	class EvalPluginManager;
	class EvalMethodManager;
	struct EvalPluginInfo;

	class EVAL_BASE_EXPORT_API EvalService
	{
	public:
		static EvalService* CreateInstance();
		static EvalService* GetInstance();
		static void DestroyInstance();

		// 评价功能
	public:
		// 设置评价结果mdb数据库文件路径
		void setEvalResultDataBasePath(const CString& dbName);		

		// 判断规程评价是否实现
		bool isRegulationEvalImpl(const CString& regulation) const;

		// 判断规程中的条款评价是否实现
		bool isClauseEvalImpl(const CString& regulation, int clauseNum) const;

		// 对注册的规程和条款进行评价
		// 如果规程没有实现，返回false
		bool doRegEval(const CString& regulation, const AcDbIntArray& clauses);

		// 插件功能
	public:
		// 加载评价插件(前提：如果验证成功)
		void loadPlugin(const CString& arxFilePath);

		// 卸载评价插件
		void unloadPlugin(const CString& arxFilePath);

		// 得到评价插件的信息
		void getPluginInfo(const CString& arxFilePath, EvalPluginInfo& info);

		// 得到所有已加载的插件的路径集合
		void getAllLoadedPluginPaths(AcStringArray& paths);

	private:
		static EvalService* m_pEvalService;
		EvalService(void);
		~EvalService(void);

		EvalMethodManager* m_pEvalMethodManager;
		EvalPluginManager* m_pEvalPluginManager;

		CString m_evalResultDataBasePath;                  // 评价结果mdb数据库路径
	};
}