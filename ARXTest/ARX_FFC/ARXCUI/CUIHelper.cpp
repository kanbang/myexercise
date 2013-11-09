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

// ʹ��acedCommand
static bool LoadCUI1( const CString& filePath )
{
    //���ز˵�
    int ret = acedCommand( RTSTR, _T( "cuiload" ), RTSTR, filePath, 0 );

    // ctrl+c���ȼ���ȡ������
    acedCommand( 0 );

    return ( ret == RTNORM );
}

static void SendESC()
{
    ////����˵��Ѵ�����ϵͳ�������ʾ����˵��ļ������Ա��뷢��ȡ������
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

    // ����esc
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

//ж�ؾֲ��˵�
bool CUIHelper::UnLoadPartialMenuUseCmd( const CString& menuGroupName )
{
    // �л�FILEDIAϵͳ����
    FileDia fd( 0 );
    CmdEcho ce( 0 );

    return UnLoadCUI1( menuGroupName );
    //return UnLoadCUI2(menuGroupName);
}

//���ؾֲ��˵�
bool CUIHelper::LoadPartialMenuUseCmd( const CString& filePath, const CString& menuGroupName )
{
    // ����еĻ���ж��
    //CUIHelper::UnLoadPartialMenuUseCmd(menuGroupName);

    // �л�FILEDIAϵͳ����
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
        long i, menuGroupNum;   //�˵�����Ŀ
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
        //e->ReportError(); // ������
        e->Delete();
        ret = false;
    }
    END_CATCH;

    return ret;
}

//���ؾֲ��˵�
bool CUIHelper::LoadPartialMenuUseCom( const CString& filePath, const CString& menuGroupName )
{
    //����еĻ���ж��
    //CUIHelper::UnLoadPartialMenuUseCom(menuGroupName);

    bool ret = true;

    TRY
    {
        long i, menuGroupNum;   //�˵�����Ŀ
        VARIANT index;
        VariantInit( &index );
        index.vt = VT_I4;

        //���ز˵�
        VARIANT BaseMenu;   //�Ƿ����Ϊ�����˵�
        VariantInit( &BaseMenu );
        BaseMenu.vt = VT_BOOL;
        BaseMenu.boolVal = FALSE;
        CAcadApplication acadApp( acedGetAcadWinApp()->GetIDispatch( TRUE ) );
        CAcadMenuGroups menuGroups( acadApp.get_MenuGroups() );
        CAcadMenuGroup menuGroup = menuGroups.Load( filePath, BaseMenu );
        // �Ѳ˵��ڲ˵�������ʾ����
        CAcadMenuBar menuBar( acadApp.get_MenuBar() ); //��ǰ�˵���
        CAcadPopupMenus popupMenus( menuGroup.get_Menus() ); //Ҫ����Ĳ˵���
        CAcadPopupMenu popupMenu;
        long curPopupMenuNum = menuBar.get_Count();   //��ǰ�˵����ϲ˵�����Ŀ
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
        //e->ReportError(); // ������
        e->Delete();
        ret = false;
    }
    END_CATCH;

    return ret;
}
