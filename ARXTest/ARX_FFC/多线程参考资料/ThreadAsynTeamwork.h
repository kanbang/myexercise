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
		DWORD dwChildId;			//>>> ���߳�ID
		FnCallbackProc fnCallback;  //>>> ���߳�֪ͨ���߳�Ҫ�ص��ĺ���
		BOOL  bAutoCallback;        //>>> ���߳̽���ʱ�Ƿ��Զ��ص�
	}ThreadInfo, * PThreadInfo;
	
	typedef struct tagThreadCallback
	{
		vector<ThreadInfo> vThreadInfo;  //>>> ���߳��б�
		HHOOK      hHook;                //>>> ���̵߳Ĺҹ�
	}ThreadCallback, * PThreadCallback;

	typedef map<DWORD, ThreadCallback> TMapThreadCallback;
	typedef map<DWORD, DWORD>          TMapThreadId;

	typedef struct tagParamInfo
	{
		DWORD dwThreadId;               //>>> �߳�ID
		BOOL  bDelete;                  //>>> �Ƿ�ɾ��
		BOOL  bNotify;                  //>>> �Ƿ�֪ͨ
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
	UINT        WM_ASYNNOTIFYMESSAGE;  //>>> �Զ�����Ϣ
	
	TMapThreadCallback m_mapThreadCallback; //>>>  ���̵߳�ID - ���̹߳ҹ������̻߳ص���Ϣ
	TMapThreadId       m_mapThreadId;       //>>>  ���߳�ID - ���߳�ID
	CRITICAL_SECTION   m_cs;	//>>>  �ٽ���
};

#endif // !defined(AFX_THREADASYNTEAMER_H__F72994B8_6838_48A0_AF47_F55FE7196A36__INCLUDED_)
