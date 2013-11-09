/*-------------------------------------------------------------------------
���ߣ�Racky Ye
�����װ����AuToCAD����Ӳ˵����Ӳ˵��ķ�����ʹ��COM�ӿ�ʵ��
��ҿ��������޸ģ�����¹��ܣ�ϣ������ཻܶ��
Email: yeguixi@126.com
--------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "acadi_i.c"

#include "ArxToolBar.h"

#import "acax18ENU.tlb" no_implementation raw_interfaces_only named_guids

//static CStringArray stcMyToolBarNames;//�洢����ӵ�CAD�����˵�������

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
        acutPrintf( _T( "buttonName'%s'���ȳ�����Χ " ), name );
#endif
        return -99;
    }
    if ( sizeof( help ) > 256 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "help'%s'���ȳ�����Χ " ), help );
#endif
        return -99;
    }

    if ( sizeof( command ) > 256 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "command'%s'���ȳ�����Χ " ), command );
#endif
        return -99;
    }

    if ( sizeof( bmp ) > 256 )
    {
#ifdef _DEBUG
        acutPrintf( _T( "bmp'%s'���ȳ�����Χ " ), bmp );
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
        acutPrintf( _T( "����Ӳ˵�%sǰû�е���AddToolBar���һ���˵���Ŀ���޷�����Ӳ˵� " ), buttonName );
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

    //�õ��˵��鼯
    hr = acadApp->get_MenuGroups( &mnuGrps );
    if ( FAILED( hr ) )
    {
        acadApp->Release();
        return false;
    }
    acadApp->Release();

    mnuGrps->get_Count( &cnt );

    //�õ��˵���
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
    // ��ȡ���˵���
    IAcadMenuGroup* mnuGrp = NULL;
    if ( !GetAcadMenuGroup( &mnuGrp ) )
        return false;

    //�õ����й�����
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

    //�õ��˵���
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

    // ��ȡ���˵���
    IAcadMenuGroup* mnuGrp = NULL;
    if ( !GetAcadMenuGroup( &mnuGrp ) )
        return ;

    //�õ����в˵���
    HRESULT hr = S_OK;
    IAcadToolbars* acToolbars = NULL;
    hr = mnuGrp->get_Toolbars( &acToolbars );

    // ���bmpDir·��û�з�б��(\)�������һ����б��
    CString path( bmpDir );
    // ȡ���һ���ַ�
    if( path.Right( 1 ) != _T( "\\" ) )
    {
        path.Format( _T( "%s\\" ), bmpDir );
    }

    VARIANT index;
    VariantInit( &index );
    V_VT( &index ) = VT_I4;

    TCHAR MenuName[256];
    int delta = 30; // ���������
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
            // ��Ӱ�ť
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
                //acutPrintf(_T("\n·��:%s"), szBmpFile);

                hr = acToolbarItem->SetBitmaps( szBmpFile, szBmpFile );
            }
        }
        //acToolbarItem->Release();
        acToolbar->Release();
    }

    acToolbars->Release();
    mnuGrp->Release();
    pMainFrame->UnlockWindowUpdate();

    //stcMyToolBarNames.Copy(m_TopMenuNames); //�����������ƴ��뾲̬�����У�ж��ʱʹ��
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
            acutPrintf( _T( "û���ҵ� %s ��Ӧ�Ĺ��������޷�ж�ء� " ), sMenu );
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

        //acutPrintf(_T("\n��%ld����ť"), i);
        hr = cmd->Delete();
        cmd->Release();
    }

    toolbar->Delete();
    toolbar->Release();
}
