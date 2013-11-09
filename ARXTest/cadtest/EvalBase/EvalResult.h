#pragma once

#include "dlimexp.h"

namespace PJ
{
	// ���۽��
	enum EvalResult
	{
		UnQualified     = 0,     // ���ϸ�
		Qualified       = 1,     // �ϸ�
		NoneResult      = 2,      // �޸�������(����������ʵ�֣���û�����ݿ�������)
		/*
		 * �����۽��ֻ��һ����ʱ����״̬
		 * ����һЩ����������ж�
		 * �������ɵ����ݿ���û�и����͵Ľ��
		 */
		UnknownResult   = 3,     // δ֪���(��������δʵ�֣����˹�ѯ��)
	};

	// ������ֵת����EvalResult��������UNKNOWN_RESULT�����
	// ֻ��Ϊ�˷���ʹ��
	extern "C" EVAL_BASE_EXPORT_API EvalResult ConvertBoolToEvalResult(bool value);
}