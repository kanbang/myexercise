#pragma once

namespace PJ
{
	// ���۲�����ؽ��
	enum EvalPluginLoadResult
	{
		EPLR_ILLEGAL_EVAL_PLUGIN = 0,       // �Ƿ������۲��
		EPLR_LOAD_SUCCESS,                  // ���سɹ�
		EPLR_CAN_NOT_CREATE_PLUGIN,         // �޷��������
		EPLR_HAS_LOAD                       // ����Ѿ�����
	};
}