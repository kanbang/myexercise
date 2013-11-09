/*-------------------------------------------------------------------------
作者：Racky Ye
该类封装了在AuToCAD上添加菜单及子菜单的方法，使用COM接口实现
大家可以任意修改，添加新功能，希望大家能多交流
Email: yeguixi@126.com
--------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "acadi_i.c"

#include "ArxToolBar.h"

#import "acax18ENU.tlb" no_implementation raw_interfaces_only named_guids

//static CStringArray stcMyToolBarNames;//存储新添加的CAD顶级菜单项名称

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ArxToolBar::ArxToolBar()
{
    m_menuCount = 0;
    m_TopMenuNames.RemoveAll();
}

ArxToolBar::~ArxToolBar()
{
    int num = m_menus.GetSize();
    for ( int i = 0; i < num; i++ )
    {
        if ( m_menus.GetAt( i ) != NULL )
            delete m_menus[i];
    }
}

int ArxToolBar::CCADToolButtonArray::AddButton( CADToolButton& button )
{
    CADToolButton* pMenu = new CADToolButton;
    _stprintf( pMenu->name, button.name );
    _stprintf( pMenu->order, button.order );
    _stprintf( pMenu->help, button.help );
    _stprintf( pMenu->bmp, button.bmp );
    return Add( pMenu );
}

int ArxToolBar::CCADToolButtonArray::AddButton( const TCHAR* name, const TCHAR* command, const TCHAR* help, const TCHAR* bmp )
{
    if ( sizeof( name ) > 256 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "buttonName'%s'长度超出范围 " ), name );
#endif
        return -99;
    }
    if ( sizeof( help ) > 256 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "help'%s'长度超出范围 " ), help );
#endif
        return -99;
    }

    if ( sizeof( command ) > 256 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "command'%s'长度超出范围 " ), command );
#endif
        return -99;
    }

    if ( sizeof( bmp ) > 256 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "bmp'%s'长度超出范围 " ), bmp );
#endif
        return -99;
    }

    CADToolButton* pMenu = new CADToolButton;

    _stprintf( pMenu->name, name );

    if ( command != NULL )
        _stprintf( pMenu->order, command );

    if( help != NULL )
        _stprintf( pMenu->help, help );

    if( bmp != NULL )
        _stprintf( pMenu->bmp, bmp );

    return Add( pMenu );
}

ArxToolBar::CCADToolButtonArray::~CCADToolButtonArray()
{
    for ( int i = 0; i < m_nSize; i++ )
    {
        if ( GetAt( i ) != NULL )
        {
            delete GetAt( i );
        }
    }
}

void ArxToolBar::AddToolButton( const TCHAR* buttonName, const TCHAR* command, const TCHAR* help, const TCHAR* bmp )
{
    assert( buttonName != NULL );
    if ( m_menuCount == 0 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "添加子菜单%s前没有调用AddToolBar添加一个菜单条目！无法添加子菜单 " ), buttonName );
#endif
        return;
    }
    m_menus[m_menuCount - 1]->AddButton( buttonName, command, help, bmp );
}

void ArxToolBar::AddSeparator()
{
    m_menus[m_menuCount - 1]->AddButton( _T( "--" ), _T( "" ), NULL, NULL );
}

void ArxToolBar::AddToolBar( const TCHAR* toolBarName )
{
    m_TopMenuNames.Add( toolBarName );
    CCADToolButtonArray* pArr = new CCADToolButtonArray;
    m_menuCount = m_menus.Add( pArr ) + 1;
}

bool ArxToolBar::GetAcadApplication( LPDISPATCH* pVal )
{
    LPDISPATCH pDispatch = acedGetAcadWinApp()->GetIDispatch( TRUE );
    if ( pDispatch == NULL )
        return false;
    *pVal = pDispatch;
    return true;
}

bool ArxToolBar::GetAcadApplication( IAcadApplication*& pAcadApp )
{
    LPDISPATCH  pDisp = NULL;

    if ( !GetAcadApplication( &pDisp ) )
        return false;

    HRESULT hr = S_OK;
    hr = pDisp->QueryInterface( IID_IAcadApplication, ( LPVOID* )&pAcadApp );
    if ( FAILED( hr ) )
        return false;

    return true;
}

bool ArxToolBar::GetAcadMenuGroup( IAcadMenuGroup**  pVal )
{
    IAcadApplication* acadApp = NULL;
    HRESULT hr = S_OK;

    if( !GetAcadApplication( acadApp ) )
        return false;

    LPDISPATCH  pTempDisp = NULL;
    IAcadMenuGroups* mnuGrps = NULL;
    long cnt = 0;

    //得到菜单组集
    hr = acadApp->get_MenuGroups( &mnuGrps );
    if ( FAILED( hr ) )
    {
        acadApp->Release();
        return false;
    }
    acadApp->Release();

    mnuGrps->get_Count( &cnt );

    //得到菜单组
    IAcadMenuGroup* mnuGrp = NULL;

    VARIANT  vtName;
    vtName.vt = VT_I4;
    BSTR  grpName;
    bool found = false ;
    for ( long i = 0; i < cnt; i++ )
    {
        vtName.lVal = i;
        hr = mnuGrps->Item( vtName, &mnuGrp );
        if ( FAILED( hr ) )
            continue;

        hr  = mnuGrp->get_Name( &grpName );
        CString cgrpName( grpName );
        SysFreeString( grpName );

        if ( cgrpName.CompareNoCase( _T( "ACAD" ) ) == 0 )
        {
            found = true;
            *pVal = mnuGrp;
            break;
        }
        else
        {
            mnuGrp->Release();
        }
    }

    mnuGrps->Release();

    return found;
}

bool ArxToolBar::GetAcadToolBar( IAcadToolbar** pVal, LPCTSTR menuname )
{
    // 获取主菜单组
    IAcadMenuGroup* mnuGrp = NULL;
    if ( !GetAcadMenuGroup( &mnuGrp ) )
        return false;

    //得到所有工具栏
    HRESULT hr = S_OK;
    IAcadToolbars* acToolbars = NULL;
    hr = mnuGrp->get_Toolbars( &acToolbars );
    if ( FAILED( hr ) )
    {
        mnuGrp->Release();
        return false;
    }

    long cnt = 0;
    hr = acToolbars->get_Count( &cnt );

    //得到菜单组
    IAcadToolbar* acToolbar = NULL;
    VARIANT  vtName;
    vtName.vt = VT_I4;

    BSTR  toolBarName;
    bool found = false ;
    for ( long i = 0; i < cnt; i++ )
    {
        vtName.lVal = i;
        hr = acToolbars->Item( vtName, &acToolbar );
        if ( FAILED( hr ) )
        {
            acToolbars->Release();
            return false;
        }

        acToolbar->get_Name( &toolBarName );
        acToolbar->Release();

        CString sToolBarName( toolBarName );
        SysFreeString( toolBarName );

        if( sToolBarName.CompareNoCase( menuname ) == 0 )
        {
            found = true;
            *pVal = acToolbar;
            break;
        }
    }
    acToolbars->Release();
    mnuGrp->Release();

    return found;
}

void ArxToolBar::CreateToolBars( const TCHAR* bmpDir )
{
    CMDIFrameWnd* pMainFrame = acedGetAcadFrame();
    pMainFrame->LockWindowUpdate();

    // 获取主菜单组
    IAcadMenuGroup* mnuGrp = NULL;
    if ( !GetAcadMenuGroup( &mnuGrp ) )
        return ;

    //得到所有菜单组
    HRESULT hr = S_OK;
    IAcadToolbars* acToolbars = NULL;
    hr = mnuGrp->get_Toolbars( &acToolbars );

    // 如果bmpDir路径没有反斜杠(\)，则添加一个反斜杠
    CString path( bmpDir );
    // 取最后一个字符
    if( path.Right( 1 ) != _T( "\\" ) )
    {
        path.Format( _T( "%s\\" ), bmpDir );
    }

    VARIANT index;
    VariantInit( &index );
    V_VT( &index ) = VT_I4;

    TCHAR MenuName[256];
    int delta = 30; // 工具栏间距
    int top = 200, left = 200;
    for ( int i = 0; i < m_menuCount; i++ )
    {
        IAcadToolbar* acToolbar = NULL;
        _tcscpy( MenuName, m_TopMenuNames[i] );
        hr = acToolbars->Add( MenuName, &acToolbar );

        acToolbar->put_Visible( true );
        acToolbar->Dock( acToolbarFloating );
        acToolbar->Float( top + i * delta, left, 1 );

        CCADToolButtonArray* Menus = m_menus[i];
        COleVariant flyOutButton;
        for ( int j = 0; j < Menus->GetSize(); j++ )
        {
            // 添加按钮
            V_I4( &index ) = ( long )j;
            CADToolButton* btn = Menus->GetAt( j );
            IAcadToolbarItem* acToolbarItem = NULL;

            if( _tcscmp( _T( "--" ), btn->name ) == 0 )
            {
                hr = acToolbar->AddSeparator( index, &acToolbarItem );
            }
            else
            {
                hr = acToolbar->AddToolbarButton( index, btn->name, btn->help, btn->order, flyOutButton, &acToolbarItem );

                TCHAR szBmpFile[256];
                _stprintf( szBmpFile, _T( "%s%s" ), path, btn->bmp );
                //acutPrintf(_T("\n路径:%s"), szBmpFile);

                hr = acToolbarItem->SetBitmaps( szBmpFile, szBmpFile );
            }
        }
        //acToolbarItem->Release();
        acToolbar->Release();
    }

    acToolbars->Release();
    mnuGrp->Release();
    pMainFrame->UnlockWindowUpdate();

    //stcMyToolBarNames.Copy(m_TopMenuNames); //将工具栏名称存入静态数组中，卸载时使用
}

void ArxToolBar::CleanUpToolBars()
{
    IAcadToolbar* acToolbar = NULL;
    for( int i = 0; i < m_TopMenuNames.GetSize(); i++ )
    {
        CString sMenu = m_TopMenuNames.GetAt( i );

        if( !GetAcadToolBar( &acToolbar, sMenu ) )
        {
#ifdef _DEBUG
            acutPrintf( _T( "没有找到 %s 对应的工具栏，无法卸载。 " ), sMenu );
#endif

            //   mnuGrp->Release();
            continue;
        }

        ClearToolBar( acToolbar );
    }

    m_TopMenuNames.RemoveAll();
}

void ArxToolBar::ClearToolBar( IAcadToolbar* toolbar )
{
    long cnt = 0l;
    HRESULT hr = toolbar->get_Count( &cnt );
    //acutPrintf(_T("\ncnt=%ld"), cnt);

    //now get Tools button
    IAcadToolbarItem* cmd = NULL;

    VARIANT vtName;
    VariantInit( &vtName );
    vtName.vt = VT_I4;

    for( long i = cnt - 1; i >= 0; i-- )
    {
        vtName.lVal = i;
        hr = toolbar->Item( vtName, &cmd );
        if ( FAILED( hr ) ) continue ;

        //acutPrintf(_T("\n第%ld个按钮"), i);
        hr = cmd->Delete();
        cmd->Release();
    }

    toolbar->Delete();
    toolbar->Release();
}
