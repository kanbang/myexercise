#pragma once

#include "dlimexp.h"

class ARXUI_DLLIMPEXP ObjectContextMenu :public AcEdUIContext
{
public:
	ObjectContextMenu(long id);
	~ObjectContextMenu(void);
	virtual void* getMenuContext(const AcRxClass *pClass, const AcDbObjectIdArray& ids) ;
	virtual void onCommand(Adesk::UInt32 cmdIndex);
	//virtual void OnUpdateMenu();

public:
	// ע�������ID
	void regMenuCmd(long id, const CString& cmd);

private:
	CMenu *m_pMenu;    // �������Ӳ˵����MFC�˵�����ʹ������Ϊ�˼���������VC�����ӵĲ˵���Դ��
	HMENU m_hDemoMenu; // �˵�������Ӧ�ľ�����������������Ҫ���صĵĲ˵������m_pDemoMenu�е�һ�

	// �������ӵ��ֶ�
private:
	CString m_className;    // ������������
	long m_menu_id;         // �˵�����ԴID
	AcGeIntArray m_cmdIds;  // �˵���id
	AcStringArray m_cmds;   // �˵���Ӧ������
};