#include "StdAfx.h"
#include "AppMenu.h"

#include "ArxMenu.h"

AppMenu::AppMenu( void )
{
    m_pMenu = new ArxMenu();
}

AppMenu::~AppMenu( void )
{
    CleanUpMenus();
    delete m_pMenu;
    m_pMenu = 0;
}

void AppMenu::AddMenu( const TCHAR* menuName )
{
    m_pMenu->AddMenu( menuName );
}

void AppMenu::AddSubMenu( const TCHAR* menuName, const TCHAR* command/*=NULL*/ )
{
    // 对命令串进行处理
    // 前面添加2个esc，末尾添加一个空格
    if( command != NULL )
    {

        CString cmd;
        cmd.Format( _T( "%s" ), command );
        cmd.Trim();

        TCHAR szCmd[256];
        _stprintf( szCmd, _T( "\003\003%s " ), cmd );
        m_pMenu->AddSubMenu( menuName, szCmd );
    }
    else
    {
        m_pMenu->AddSubMenu( menuName, NULL );
    }
}

void AppMenu::AddSeparator()
{
    m_pMenu->AddSeparator();
}

void AppMenu::AddSubSeparator()
{
    m_pMenu->AddSubSeparator();
}

void AppMenu::CreateMenus()
{
    m_pMenu->CreateMenus();
}

void AppMenu::CleanUpMenus()
{
    m_pMenu->CleanUpMenus();
}