#include "StdAfx.h"
#include "CUIHelper.h"

#include "FileDia.h"
#include "CmdEcho.h"

#include "CAcadApplication.h"
#include "CAcadDocument.h"
#include "CAcadModelSpace.h"
#include "CAcadMenuBar.h"
#include "CAcadMenuGroup.h"
#include "CAcadMenuGroups.h"
#include "CAcadPopupMenu.h"
#include "CAcadPopupMenus.h"

//#import "acax18enu.tlb" no_implementation raw_interfaces_only named_guids

// 使用acedCommand
static bool LoadCUI1( const CString& filePath )
{
    //加载菜单
    int ret = acedCommand( RTSTR, _T( "cuiload" ), RTSTR, filePath, 0 );

    // ctrl+c，等价于取消命令
    acedCommand( 0 );

    return ( ret == RTNORM );
}

static void SendESC()
{
    ////如果菜单已存在则系统会继续提示输入菜单文件，所以必须发送取消命令
    TCHAR menuCmd[3];
    menuCmd[0] = 27;
    menuCmd[1] = _T( '\0' );
    acDocManager->sendStringToExecute( curDoc(), menuCmd );
}

static bool LoadCUI2( const CString& filePath )
{
    CString cmd;
    cmd.Format( _T( "cuiload %s " ), filePath );
    Acad::ErrorStatus es = acDocManager->sendStringToExecute( curDoc(), cmd, false, false, false );

    // 发送esc
    SendESC();

    return ( Acad::eOk == es );
}

static bool UnLoadCUI1( const CString& menuGroupName )
{
    return ( RTNORM == acedCommand( RTSTR, _T( "cuiunload" ), RTSTR, menuGroupName, 0 ) );
}

static bool UnLoadCUI2( const CString& menuGroupName )
{
    CString cmd;
    cmd.Format( _T( "cuiunload %s " ), menuGroupName );
    return ( Acad::eOk == acDocManager->sendStringToExecute( curDoc(), cmd, false, false, false ) );
}

//卸载局部菜单
bool CUIHelper::UnLoadPartialMenuUseCmd( const CString& menuGroupName )
{
    // 切换FILEDIA系统变量
    FileDia fd( 0 );
    CmdEcho ce( 0 );

    return UnLoadCUI1( menuGroupName );
    //return UnLoadCUI2(menuGroupName);
}

//加载局部菜单
bool CUIHelper::LoadPartialMenuUseCmd( const CString& filePath, const CString& menuGroupName )
{
    // 如果有的话先卸载
    //CUIHelper::UnLoadPartialMenuUseCmd(menuGroupName);

    // 切换FILEDIA系统变量
    FileDia fd( 0 );
    CmdEcho ce( 0 );

    return LoadCUI1( filePath );
    //return LoadCUI2(filePath);
}

bool CUIHelper::UnLoadPartialMenuUseCom( const CString& menuGroupName )
{
    bool ret = true;

    TRY
    {
        long i, menuGroupNum;   //菜单组数目
        VARIANT index;
        VariantInit( &index );
        index.vt = VT_I4;
        CAcadApplication acadApp( acedGetAcadWinApp()->GetIDispatch( TRUE ) );
        CAcadMenuGroups menuGroups( acadApp.get_MenuGroups() );
        menuGroupNum = menuGroups.get_Count();
        for ( i = 0; i < menuGroupNum; i++ )
        {
            index.lVal = i;
            CAcadMenuGroup menuGroup = menuGroups.Item( index );
            if ( menuGroup.get_Name() == menuGroupName )
            {
                menuGroup.Unload();
                break;
            }
        }
    }
    CATCH( COleDispatchException, e )
    {
        //e->ReportError(); // 调试用
        e->Delete();
        ret = false;
    }
    END_CATCH;

    return ret;
}

//加载局部菜单
bool CUIHelper::LoadPartialMenuUseCom( const CString& filePath, const CString& menuGroupName )
{
    //如果有的话先卸载
    //CUIHelper::UnLoadPartialMenuUseCom(menuGroupName);

    bool ret = true;

    TRY
    {
        long i, menuGroupNum;   //菜单组数目
        VARIANT index;
        VariantInit( &index );
        index.vt = VT_I4;

        //加载菜单
        VARIANT BaseMenu;   //是否加载为基础菜单
        VariantInit( &BaseMenu );
        BaseMenu.vt = VT_BOOL;
        BaseMenu.boolVal = FALSE;
        CAcadApplication acadApp( acedGetAcadWinApp()->GetIDispatch( TRUE ) );
        CAcadMenuGroups menuGroups( acadApp.get_MenuGroups() );
        CAcadMenuGroup menuGroup = menuGroups.Load( filePath, BaseMenu );
        // 把菜单在菜单条上显示出来
        CAcadMenuBar menuBar( acadApp.get_MenuBar() ); //当前菜单条
        CAcadPopupMenus popupMenus( menuGroup.get_Menus() ); //要加入的菜单条
        CAcadPopupMenu popupMenu;
        long curPopupMenuNum = menuBar.get_Count();   //当前菜单条上菜单的数目
        long n = popupMenus.get_Count();
        for ( long i = 0; i < n; i++ )
        {
            index.lVal = i;
            popupMenu = popupMenus.Item( index );
            index.lVal = i + curPopupMenuNum;
            popupMenu.InsertInMenuBar( index );
        }
    }
    CATCH( COleDispatchException, e )
    {
        //e->ReportError(); // 调试用
        e->Delete();
        ret = false;
    }
    END_CATCH;

    return ret;
}
