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

		// ���۹���
	public:
		// �������۽��mdb���ݿ��ļ�·��
		void setEvalResultDataBasePath(const CString& dbName);		

		// �жϹ�������Ƿ�ʵ��
		bool isRegulationEvalImpl(const CString& regulation) const;

		// �жϹ���е����������Ƿ�ʵ��
		bool isClauseEvalImpl(const CString& regulation, int clauseNum) const;

		// ��ע��Ĺ�̺������������
		// ������û��ʵ�֣�����false
		bool doRegEval(const CString& regulation, const AcDbIntArray& clauses);

		// �������
	public:
		// �������۲��(ǰ�᣺�����֤�ɹ�)
		void loadPlugin(const CString& arxFilePath);

		// ж�����۲��
		void unloadPlugin(const CString& arxFilePath);

		// �õ����۲������Ϣ
		void getPluginInfo(const CString& arxFilePath, EvalPluginInfo& info);

		// �õ������Ѽ��صĲ����·������
		void getAllLoadedPluginPaths(AcStringArray& paths);

	private:
		static EvalService* m_pEvalService;
		EvalService(void);
		~EvalService(void);

		EvalMethodManager* m_pEvalMethodManager;
		EvalPluginManager* m_pEvalPluginManager;

		CString m_evalResultDataBasePath;                  // ���۽��mdb���ݿ�·��
	};
}