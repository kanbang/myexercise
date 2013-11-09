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

// 日志消息
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
 * 使用全局变量的原因
 * 通过CreateThread的第4个参数LPVOID lpParameter
 * 传递数据在多线程中有问题???
 * 特别是CString会出现关于tscpy_s的一个错误
 * 原因：在传递参数过程中，CString的拷贝可能发生了问题
 */

// 全局fluent线程数据
extern FluentThreadData global_fluent_thread_data;

// 全局gambit线程数据
extern GambitThreadData global_gambit_thread_data;

// 全局日志消息数据
extern LogMsg global_log_msg;

// 全局ffc线程数据
extern FFCThreadData global_ffc_thread_data;

// 全局变量--用于表示计算阶段
extern int ffc_step;

// 用户是否要求线程结束
extern bool IsUserAskForThreadEnd();

// 设置线程结束标记
extern void UserAskForThreadEnd(bool flag);
