#include "StdAfx.h"
#include "UIHelper.h"

#include "Resource.h"

#include "../ArxUI/MenuHelper.h"
#include "../ArxUI/ToolBarHelper.h"
#include "../ArxUI/ObjectContextMenuHelper.h"

void UIHelper::RegisterAppMenu()
{
    MenuHelper menu;
    menu.AddMenu( _T( "�ҵĲ˵�" ) );
    menu.AddSubMenu( _T( "�����Ի���" ), _T( "_DLG " ) );
    menu.AddSubMenu( _T( "��������" ), _T( "_DRAWL " ) );
    menu.AddSubMenu( _T( "*Line" ) ); // ��"*"�ű�ʾ�Ƕ����Ӳ˵�
    menu.AddSubMenu( _T( "$CIRCLE" ), _T( "_CIRCLE " ) ); //��"$"�ű�ʾ�����Ӳ˵��µĲ˵���
    menu.AddSubSeparator();
    menu.AddSubMenu( _T( "$PLINE" ), _T( "_PLINE " ) ); //��"$"�ű�ʾ�����Ӳ˵��µĲ˵���
    menu.AddSeparator();
    menu.AddSubMenu( _T( "ѡ���ļ���" ), _T( "_SELFOLDER " ) );
    menu.AddSubMenu( _T( "*TEXT" ) ); // ��"*"�ű�ʾ�Ƕ����Ӳ˵�
    menu.AddSubMenu( _T( "$MTEXT" ), _T( "_TEXT " ) ); //��"$"�ű�ʾ�����Ӳ˵��µĲ˵���
    menu.CreateMenus();
}

void UIHelper::UnRegisterAppMenu()
{
    MenuHelper::CleanUpMenus();
}

void UIHelper::RegisterAppToolBar()
{
    ToolBarHelper toolbar;
    toolbar.AddToolBar( _T( "ͨ��ͼԪ" ) );
    toolbar.AddToolButton( _T( "���" ), _T( "JL.DrawTunnel" ), _T( "�������" ), _T( "1.bmp" ) );
    toolbar.AddToolButton( _T( "����" ), _T( "JL.DrawFan" ), _T( "���Ʒ���" ), _T( "2.bmp" ) );
    toolbar.AddSeparator();
    toolbar.AddToolButton( _T( "���վ" ), _T( "JL.DrawStation" ), _T( "���Ʋ��վ" ), _T( "3.bmp" ) );
    toolbar.AddToolBar( _T( "��˹ͼԪ" ) );
    toolbar.AddToolButton( _T( "��ú���" ), _T( "JL.DrawCoalHole" ), _T( "���Ƽ�ú���" ), _T( "4.bmp" ) );
    toolbar.CreateToolBars();
}

void UIHelper::UnRegisterAppToolBar()
{
    ToolBarHelper::CleanUpToolBars();
}

void UIHelper::RegisterObjectContextMenu( const void* appId )
{
    ObjectContextMenu* pObjMenu1 = new ObjectContextMenu( _T( "AcDbLine" ), IDR_MENU1 );
    pObjMenu1->regMenuCmd( ID_32769, _T( "CIRCLE" ) );

    ObjectContextMenu* pObjMenu2 = new ObjectContextMenu( _T( "AcDbLine" ), UI_LOADER_MNU_OBJECT_WS );
    pObjMenu2->regMenuCmd( UI_LOADER_MNU_ADVANCE_WS, _T( "LINE" ) );


    ObjectContextMenuHelper::RegObjectContextMenu( pObjMenu1, appId );
    ObjectContextMenuHelper::RegObjectContextMenu( pObjMenu2, appId );
}

void UIHelper::UnRegisterObjectContextMenu()
{
    ObjectContextMenuHelper::UnRegAllObjectContextMenu();
}