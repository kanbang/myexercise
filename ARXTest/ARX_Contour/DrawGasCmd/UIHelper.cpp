#include "StdAfx.h"
#include "UIHelper.h"

#include "DataList_DockBar.h"

#include "../ArxHelper/HelperClass.h"

DataList_DockBar* pDataList_DockBar = 0;

void UIHelper::ShowDataListDockBar()
{
    CAcModuleResourceOverride myResources;

    CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

    if( pDataList_DockBar == 0 )
    {
        pDataList_DockBar = new DataList_DockBar();
        pDataList_DockBar->Create ( pAcadFrame, _T( "图形数据列表" ) ) ;
        pDataList_DockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
        pDataList_DockBar->RestoreControlBar () ;
    }
    pAcadFrame->ShowControlBar( pDataList_DockBar, TRUE, FALSE );
}

void UIHelper::DestroyDataListDockBar()
{
    if( pDataList_DockBar != 0 )
    {
        pDataList_DockBar->DestroyWindow();
        delete pDataList_DockBar;
        pDataList_DockBar = 0;
    }
}