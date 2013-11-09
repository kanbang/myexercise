#include "stdafx.h"
#include "ObjectContextMenu.h"
#include "resource.h"

// ObjectARX中右键(快捷)菜单的实现方法
// 来源：http://www.3s8.cn/Autocad/cadjiaocheng/200705/1390.html

ObjectContextMenu::ObjectContextMenu( const CString& className, long id ) : m_menu_id( id ), m_className( className )
{
    // 切换资源
    CAcModuleResourceOverride myResources;

    // 创建一个菜单对象
    m_pMenu = new CMenu;

    // 使用创建的菜单对象加载在资源编辑器中编辑好的资源
    m_pMenu->LoadMenu( m_menu_id );
}

ObjectContextMenu::~ObjectContextMenu( void )
{
    if( m_pMenu )
    {
        delete m_pMenu;
    }
}

// 添加显示我们自己菜单项
void* ObjectContextMenu::getMenuContext( const AcRxClass* pClass, const AcDbObjectIdArray& ids )
{
    m_hDemoMenu = m_pMenu->GetSubMenu( 0 )->GetSafeHmenu(); // 显示已经加载的菜单m_pMenu中的第一个子菜单
    return &m_hDemoMenu;
}

// 处理命令执行
void ObjectContextMenu::onCommand( Adesk::UInt32 cmdIndex )
{
    //acDocManager->pushResourceHandle(_hdllInstance);     // 切换当前使用的资源
    //CString strMenuTitle;
    //CString strPrompt;
    //m_pMenu->GetMenuString(cmdIndex, strMenuTitle, MF_BYCOMMAND); // 获取一所选菜单项的文本标题
    //strPrompt.Format(_T("\n您已经选取了菜单:%s\n"),strMenuTitle);
    //acutPrintf(strPrompt);            // 显示哪一个菜单项被选择
    //acedPostCommandPrompt();           // 显示命令提示
    //acDocManager->popResourceHandle();         // 将资源切换回来

    //if (cmdIndex == UI_LOADER_MNU_ADVANCE_WS)
    //	cmdStr = _T("LINE");
    //else if (cmdIndex == ARXDBG_APPCMD_DB_INFO)
    //	cmdStr = _T("ArxDbgSnoopDb");

    LPTSTR cmdStr = NULL;

    // 查找菜单项ID
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

// 修改菜单项的显示状态
//void ObjectContextMenu::OnUpdateMenu()
//{
//	// m_pDemoMenu->EnableMenuItem(IDR_DEMO_DEFAULT_MENU_ITEM1,MF_GRAYED);  // 使菜单变灰
//	m_pMenu->EnableMenuItem(IDR_MENU1,MF_ENABLED);  // 使用菜单项
//	// m_pDemoMenu->CheckMenuItem(IDR_DEMO_DEFAULT_MENU_ITEM3, MF_BYCOMMAND|MF_CHECKED);//复选菜单项
//}