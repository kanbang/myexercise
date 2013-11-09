#pragma once

#include "dlimexp.h"

class ARXCUI_DLLIMPEXP CUIHelper
{
public:
	// ʹ�����ʽ���ڵ�����
	// 1) �����kLoadAppMsg��Ϣ�е��ø÷���,�˵�������ʾ,ֻ����ʾ������
	// 2) cadֱ���˳���ʱ����ж�ز˵�
	//	 2.1) acedCommandִ����Чʧ��
	//	 2.2) sendStringToExecute����ִ�гɹ����������޷�ж�ز˵�
	// 3) �ڷ�application��������
	//	 3.1) acedCommandִ����Ч
	//	 3.2) sendStringToExecuteִ�л���ʾcuiload�Ի���(filedia=0������������)
	static bool UnLoadPartialMenuUseCmd(const CString& menuGroupName);
	static bool LoadPartialMenuUseCmd(const CString& filePath, const CString& menuGroupName);

	// ʹ��com��ʽ���ڵ�����
	// 1) cadֱ���˳���ʱ����ж�ز˵�
	// 2) ʹ��comж��cui��ʱ�򣬲˵��͹���������ʧ��
	//    ����acad.cuix(���Զ����ļ�)����Ȼ�����˼�¼
	// �²�ԭ��ʹ��com��ʽ�޷�Ӱ�쵽acad.cuix
	static bool UnLoadPartialMenuUseCom(const CString& menuGroupName);
	static bool LoadPartialMenuUseCom(const CString& filePath, const CString& menuGroupName);
};
