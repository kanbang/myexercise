#pragma once

#include "config.h"

extern CString GetAppPathDir();
extern CString BuildPath(const CString& dir, const CString& fileName);
//extern CString BuildArxFileName(const CString& arxModuleName);
//extern CString BuildServiceName(const CString& arxModuleName);

extern bool loadArxModule(const CString& arxModuleName);
extern void unloadArxModule(const CString& arxModuleName);

extern bool AddSupportPath(const CString& arxPath);
extern bool RemoveSupportPath(const CString& arxPath);

// ʹ�����ʽ���ڵ�����
// 1) �����kLoadAppMsg��Ϣ�е��ø÷���,�˵�������ʾ,ֻ����ʾ������
// 2) cadֱ���˳���ʱ����ж�ز˵�
//	 2.1) acedCommandִ����Чʧ��
//	 2.2) sendStringToExecute����ִ�гɹ����������޷�ж�ز˵�
// 3) �ڷ�application��������
//	 3.1) acedCommandִ����Ч
//	 3.2) sendStringToExecuteִ�л���ʾcuiload�Ի���(filedia=0������������)
extern bool UnLoadPartialMenuUseCmd(const CString& menuGroupName);
extern bool LoadPartialMenuUseCmd(const CString& filePath, const CString& menuGroupName);

// ʹ��com��ʽ���ڵ�����
// 1) cadֱ���˳���ʱ����ж�ز˵�
// 2) ʹ��comж��cui��ʱ�򣬲˵��͹���������ʧ��
//    ����acad.cuix(���Զ����ļ�)����Ȼ�����˼�¼
// �²�ԭ��ʹ��com��ʽ�޷�Ӱ�쵽acad.cuix
extern bool UnLoadPartialMenuUseCom(const CString& menuGroupName);
extern bool LoadPartialMenuUseCom(const CString& filePath, const CString& menuGroupName);

extern void AddLoaderMenu();
extern void RemoveLoaderMenu();