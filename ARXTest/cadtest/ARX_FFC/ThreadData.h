#pragma once

struct GambitThreadData
{
	HANDLE hThread;
	HANDLE hProcess;
	CString mshFile;
	CString errorFile;
};

struct FluentThreadData
{
	HANDLE hThread;
	HANDLE hProcess;
	CString datFile;
	CString errorFile;
};

// ��־��Ϣ
struct LogMsg
{
	CString str;
	COLORREF color;
};

struct FFCThreadData
{
	HWND hWnd;
	int state;
};

/*
 * ʹ��ȫ�ֱ�����ԭ��
 * ͨ��CreateThread�ĵ�4������LPVOID lpParameter
 * ���������ڶ��߳���������???
 * �ر���CString����ֹ���tscpy_s��һ������
 * ԭ���ڴ��ݲ��������У�CString�Ŀ������ܷ���������
 */

// ȫ��fluent�߳�����
extern FluentThreadData global_fluent_thread_data;

// ȫ��gambit�߳�����
extern GambitThreadData global_gambit_thread_data;

// ȫ����־��Ϣ����
extern LogMsg global_log_msg;

// ȫ��ffc�߳�����
extern FFCThreadData global_ffc_thread_data;

// ȫ�ֱ���--���ڱ�ʾ����׶�
extern int ffc_step;

// �û��Ƿ�Ҫ���߳̽���
extern bool IsUserAskForThreadEnd();

// �����߳̽������
extern void UserAskForThreadEnd(bool flag);
