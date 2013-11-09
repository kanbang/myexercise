#pragma once

#include "dlimexp.h"
#include "ObjectContextMenu.h"

/*
 �Ҽ�����˵�������
 ʹ�÷�����
 // 1) ��������˵����󣬴�������������ƺͲ˵���ԴID
 ObjectContextMenu* pObjMenu = new ObjectContextMenu(_T("AcDbLine"), UI_LOADER_MNU_OBJECT_WS);

 // 2) �����˵��������
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS1, _T("test1"));
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS2, _T("test2"));
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS3, _T("test3"));
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS4, _T("test4"));

 // 3) ע��˵�
 // appId������kLoadAppMsg��Ϣ����(pkt)
 ObjectContextMenuHelper::RegObjectContextMenu(pObjMenu, appId);

 // 4) ���Ҫж�ز˵��������UnRegObjectContextMenu()����

 */
class ARXUI_DLLIMPEXP ObjectContextMenuHelper
{
public:
	// ע�����˵�
	static bool RegObjectContextMenu(const CString& className, ObjectContextMenu* pObjMenu, const void *appId);
	// ɾ������˵�
	static bool UnRegObjectContextMenu(const CString& className, ObjectContextMenu* pObjMenu);
};
