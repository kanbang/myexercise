// ThreadAsynTeamer.h: interface for the CThreadAsynTeamer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADASYNTEAMER_H__F72994B8_6838_48A0_AF47_F55FE7196A36__INCLUDED_)
#define AFX_THREADASYNTEAMER_H__F72994B8_6838_48A0_AF47_F55FE7196A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CThreadAsynTeamwork  
{
private:
	CThreadAsynTeamwork();
	~CThreadAsynTeamwork();

private:
	typedef UINT (*FnCallbackProc)(PVOID pExtra);

	typedef struct tagThreadInfo
	{
		DWORD dwChildId;			//>>> 子线程ID
		FnCallbackProc fnCallback;  //>>> 子线程通知父线程要回调的函数
		BOOL  bAutoCallback;        //>>> 子线程结束时是否自动回调
	}ThreadInfo, * PThreadInfo;
	
	typedef struct tagThreadCallback
	{
		vector<ThreadInfo> vThreadInfo;  //>>> 子线程列表
		HHOOK      hHook;                //>>> 父线程的挂钩
	}ThreadCallback, * PThreadCallback;

	typedef map<DWORD, ThreadCallback> TMapThreadCallback;
	typedef map<DWORD, DWORD>          TMapThreadId;

	typedef struct tagParamInfo
	{
		DWORD dwThreadId;               //>>> 线程ID
		BOOL  bDelete;                  //>>> 是否删除
		BOOL  bNotify;                  //>>> 是否通知
	}ParamInfo, *PParamInfo;

public:
	inline static CThreadAsynTeamwork* Instance()
	{
		if (m_pThreadAsyn == NULL)
		{
			m_pThreadAsyn = new CThreadAsynTeamwork;
		}
		
		return m_pThreadAsyn;
	}

	inline static void Release()
	{
		if (m_pThreadAsyn != NULL)
			delete m_pThreadAsyn;

		m_pThreadAsyn = NULL;
	}

	int SetCallBackProc(DWORD dwChild, FnCallbackProc fnCallback, BOOL bAutoCallback = FALSE);
	BOOL Notify(PVOID pExtra = NULL);
	
private:
	static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
	static void ThreadDetach(DWORD dwThreadId);
	
private:
	static CThreadAsynTeamwork* m_pThreadAsyn;
	UINT        WM_ASYNNOTIFYMESSAGE;  //>>> 自定义消息
	
	TMapThreadCallback m_mapThreadCallback; //>>>  父线程的ID - 父线程挂钩和子线程回调信息
	TMapThreadId       m_mapThreadId;       //>>>  子线程ID - 父线程ID
	CRITICAL_SECTION   m_cs;	//>>>  临界区
};

#endif // !defined(AFX_THREADASYNTEAMER_H__F72994B8_6838_48A0_AF47_F55FE7196A36__INCLUDED_)
