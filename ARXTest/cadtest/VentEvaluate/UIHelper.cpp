#include "StdAfx.h"
#include "UIHelper.h"

#include "DBFieldNames.h"

#include "TunnelDockBarChildDlg.h"
#include "WSDockBarChildDlg.h"
#include "TWSDockBarChildDlg.h"
#include "WindStationDockBar.h"
#include "CycleList_DockBar.h"
#include "CCList_DockBar.h"
#include "VentShaftList_DockBar.h"

#include "MineInfoDlg.h"
#include "ProjectInfoDlg.h"
#include "InstrumentDlg.h"
#include "EvalDlg.h"

#include "DataInput_DockBar.h"
#include "DataList_DockBar.h"

#include "../ArxHelper/HelperClass.h"

DataList_DockBar* pDataList_DockBar = 0;
DataInput_DockBar* pDataInput_DockBar = 0;
WindStationDockBar* pWindStationDockBar = 0;
CycleList_DockBar* pCycleList_DockBar = 0;
CCList_DockBar* pCCList_DockBar = 0;
VentShaftList_DockBar* pVentShaftList_DockBar = 0;

void UIHelper::ShowMineInfoDlg()
{
    // 切换资源
    CAcModuleResourceOverride myResources;

    MineInfoDlg dlg;
    dlg.DoModal();
}

void UIHelper::ShowProjectInfoDlg()
{
    // 切换资源
    CAcModuleResourceOverride myResources;

    ProjectInfoDlg dlg;
    dlg.DoModal();
}


void UIHelper::ShowDataInputDockBar()
{
    static int n = 0;

    CAcModuleResourceOverride myResources;

    CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

    if( pDataInput_DockBar == 0 )
    {
        pDataInput_DockBar = new DataInput_DockBar();
        pDataInput_DockBar->Create ( pAcadFrame, _T( "巷道、工作面、掘进、硐室参数" ) ) ;
        pDataInput_DockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
        pDataInput_DockBar->RestoreControlBar () ;
    }
    //pWindStationDockBar->RestoreControlBar();
    pAcadFrame->ShowControlBar( pDataInput_DockBar, TRUE, FALSE );
}

void UIHelper::DestroyDataInputDockBar()
{
    if( pDataInput_DockBar != 0 )
    {
        pDataInput_DockBar->DestroyWindow();
        delete pDataInput_DockBar;
        pDataInput_DockBar = 0;
    }
}

void UIHelper::ShowWindStationDockBar()
{
    CAcModuleResourceOverride myResources;

    CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

    if( pWindStationDockBar == 0 )
    {
        pWindStationDockBar = new WindStationDockBar();
        pWindStationDockBar->Create ( pAcadFrame, _T( "测风站参数" ) ) ;
        pWindStationDockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
        pWindStationDockBar->RestoreControlBar () ;
    }
    //pWindStationDockBar->RestoreControlBar();
    pAcadFrame->ShowControlBar( pWindStationDockBar, TRUE, FALSE );
}

void UIHelper::DestroyWindStationDockBar()
{
    if( pWindStationDockBar != 0 )
    {
        pWindStationDockBar->DestroyWindow();
        delete pWindStationDockBar;
        pWindStationDockBar = 0;
    }
}

void UIHelper::ShowInstrumentDlg()
{
    CAcModuleResourceOverride myResources;
    InstrumentDlg dlg;
    dlg.DoModal();
}

void UIHelper::ShowEvalDlg()
{
    static CString m_regulateDB;
    static CString m_outputReport;

    CAcModuleResourceOverride myResources;
    EvalDlg dlg;
    dlg.m_regulateDB = m_regulateDB;
    dlg.m_outputReport = m_outputReport;
    dlg.DoModal();

    m_regulateDB = dlg.m_regulateDB;
    m_outputReport = dlg.m_outputReport;
}

void UIHelper::ShowCycleListDockBar()
{
    CAcModuleResourceOverride myResources;

    CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

    if( pCycleList_DockBar == 0 )
    {
        pCycleList_DockBar = new CycleList_DockBar();
        pCycleList_DockBar->Create ( pAcadFrame, _T( "单向回路列表" ) ) ;
        pCycleList_DockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
        pCycleList_DockBar->RestoreControlBar () ;
    }
    //pCycleList_DockBar->RestoreControlBar();
    pAcadFrame->ShowControlBar( pCycleList_DockBar, TRUE, FALSE );
}

void UIHelper::DestroyCycleListDockBar()
{
    if( pCycleList_DockBar != 0 )
    {
        pCycleList_DockBar->DestroyWindow();
        delete pCycleList_DockBar;
        pCycleList_DockBar = 0;
    }
}

void UIHelper::ShowCCListDockBar()
{
    CAcModuleResourceOverride myResources;

    CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

    if( pCCList_DockBar == 0 )
    {
        pCCList_DockBar = new CCList_DockBar();
        pCCList_DockBar->Create ( pAcadFrame, _T( "连通块列表" ) );
        pCCList_DockBar->EnableDocking ( CBRS_ALIGN_ANY );
        pCCList_DockBar->RestoreControlBar();
    }
    //pCCList_DockBar->RestoreControlBar();
    pAcadFrame->ShowControlBar( pCCList_DockBar, TRUE, FALSE );
}

void UIHelper::DestroyCCListDockBar()
{
    if( pCCList_DockBar != 0 )
    {
        pCCList_DockBar->DestroyWindow();
        delete pCCList_DockBar;
        pCCList_DockBar = 0;
    }
}

void UIHelper::ShowVentShaftDockBar()
{
    CAcModuleResourceOverride myResources;

    CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

    if( pVentShaftList_DockBar == 0 )
    {
        pVentShaftList_DockBar = new VentShaftList_DockBar();
        pVentShaftList_DockBar->Create ( pAcadFrame, _T( "风井自动识别" ) ) ;
        pVentShaftList_DockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
        pVentShaftList_DockBar->RestoreControlBar () ;
    }
    //pVentShaftList_DockBar->RestoreControlBar();
    pAcadFrame->ShowControlBar( pVentShaftList_DockBar, TRUE, FALSE );
}

void UIHelper::DestroyVentShaftDockBar()
{
    if( pVentShaftList_DockBar != 0 )
    {
        pVentShaftList_DockBar->DestroyWindow();
        delete pVentShaftList_DockBar;
        pVentShaftList_DockBar = 0;
    }
}

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