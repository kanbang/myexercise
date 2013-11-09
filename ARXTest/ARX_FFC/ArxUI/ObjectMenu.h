#pragma once

#include "dlimexp.h"

class ObjectContextMenu;

class ARXUI_DLLIMPEXP ObjectMenu
{
public:
	ObjectMenu(long id);
	~ObjectMenu(void);

	// ע�������ID
	void RegMenuCmd(long id, const CString& cmd);

	// ���ö�����������
	void SetObjectType(const CString& className);

	// ��������˵�
	bool CreateObjectMenu(const void* appId);

private:
	// ɾ������˵�
	bool ClearObjectMenu();

private:
	CString m_className;
	ObjectContextMenu* m_pObjMenu;
};
