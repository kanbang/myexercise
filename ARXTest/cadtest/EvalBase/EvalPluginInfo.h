#pragma once

#include "dlimexp.h"

namespace PJ
{
	// ���۲��������Ϣ
	struct EVAL_BASE_EXPORT_API EvalPluginInfo
	{
		CString name;              // �������--���ݿ��е���Ϣ������
		CString author;            // ������
		CString date;              // ����ʱ��
		CString regulation;        // �������--ʵ������(����"ú��ȫ���2004")
		CString clauseImpl;		   // ��ʵ�ֵ����Χ
	};
}