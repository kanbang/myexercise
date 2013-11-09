#include "stdafx.h"
#include "ObjectContextMenu.h"
#include "resource.h"

// ObjectARX���Ҽ�(���)�˵���ʵ�ַ���
// ��Դ��http://www.3s8.cn/Autocad/cadjiaocheng/200705/1390.html

ObjectContextMenu::ObjectContextMenu( const CString& className, long id ) : m_menu_id( id ), m_className( className )
{
    // �л���Դ
    CAcModuleResourceOverride myResources;

    // ����һ���˵�����
    m_pMenu = new CMenu;

    // ʹ�ô����Ĳ˵������������Դ�༭���б༭�õ���Դ
    m_pMenu->LoadMenu( m_menu_id );
}

ObjectContextMenu::~ObjectContextMenu( void )
{
    if( m_pMenu )
    {
        delete m_pMenu;
    }
}

// �����ʾ�����Լ��˵���
void* ObjectContextMenu::getMenuContext( const AcRxClass* pClass, const AcDbObjectIdArray& ids )
{
    m_hDemoMenu = m_pMenu->GetSubMenu( 0 )->GetSafeHmenu(); // ��ʾ�Ѿ����صĲ˵�m_pMenu�еĵ�һ���Ӳ˵�
    return &m_hDemoMenu;
}

// ��������ִ��
void ObjectContextMenu::onCommand( Adesk::UInt32 cmdIndex )
{
    //acDocManager->pushResourceHandle(_hdllInstance);     // �л���ǰʹ�õ���Դ
    //CString strMenuTitle;
    //CString strPrompt;
    //m_pMenu->GetMenuString(cmdIndex, strMenuTitle, MF_BYCOMMAND); // ��ȡһ��ѡ�˵�����ı�����
    //strPrompt.Format(_T("\n���Ѿ�ѡȡ�˲˵�:%s\n"),strMenuTitle);
    //acutPrintf(strPrompt);            // ��ʾ��һ���˵��ѡ��
    //acedPostCommandPrompt();           // ��ʾ������ʾ
    //acDocManager->popResourceHandle();         // ����Դ�л�����

    //if (cmdIndex == UI_LOADER_MNU_ADVANCE_WS)
    //	cmdStr = _T("LINE");
    //else if (cmdIndex == ARXDBG_APPCMD_DB_INFO)
    //	cmdStr = _T("ArxDbgSnoopDb");

    LPTSTR cmdStr = NULL;

    // ���Ҳ˵���ID
    int pos = m_cmdIds.find( cmdIndex );
    if( pos != -1 )
    {
        cmdStr = ( LPTSTR )m_cmds[pos].kACharPtr();
    }

    if ( cmdStr != NULL )
    {
        CString str;
        str.Format( _T( "%s\n" ), cmdStr );
        acDocManager->sendStringToExecute( acDocManager->curDocument(), str, false, false );
    }
}

void ObjectContextMenu::regMenuCmd( long id, const CString& cmd )
{
    m_cmdIds.append( id );
    m_cmds.append( cmd );
}

CString ObjectContextMenu::getClassName() const
{
    return m_className;
}

// �޸Ĳ˵������ʾ״̬
//void ObjectContextMenu::OnUpdateMenu()
//{
//	// m_pDemoMenu->EnableMenuItem(IDR_DEMO_DEFAULT_MENU_ITEM1,MF_GRAYED);  // ʹ�˵����
//	m_pMenu->EnableMenuItem(IDR_MENU1,MF_ENABLED);  // ʹ�ò˵���
//	// m_pDemoMenu->CheckMenuItem(IDR_DEMO_DEFAULT_MENU_ITEM3, MF_BYCOMMAND|MF_CHECKED);//��ѡ�˵���
//}