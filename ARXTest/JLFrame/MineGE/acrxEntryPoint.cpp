#include "StdAfx.h"

#include "config.h"
#include "HelperClass.h"
#include "CurDrawTool.h"
#include "ReactorHelper.h"
#include "../ArxHelper/HelperClass.h"

// 定义注册服务名称
#ifndef MINEGE_SERVICE_NAME
#define MINEGE_SERVICE_NAME _T("MINEGE_SERVICE_NAME")
#endif

class CMineGEApp : public AcRxArxApp
{

public:
    CMineGEApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( MINEGE_SERVICE_NAME );
        ReactorHelper::CreateDocumentReactorMap();

        acutPrintf( _T( "\nMineGE::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( MINEGE_SERVICE_NAME );
        ReactorHelper::RemoveDocumentReactorMap();

        acutPrintf( _T( "\nMineGE::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // 注册词典
        ArxDataTool::RegDict( PROPERTY_DATA_FIELD_DICT );
        ArxDataTool::RegDict( STRING_LIST_DICT );
        ArxDataTool::RegDict( INT_LIST_DICT );
        ArxDataTool::RegDict( OBJECT_LIST_DICT );
        ArxDataTool::RegDict( OBJECT_LIST_DATA_DICT );
        ArxDataTool::RegDict( GLOBAL_SINGLE_INFO_DICT );
        ArxDataTool::RegDict( CUR_DRAW_DICT );

        // 注册扩展数据组名
        ArxDataTool::RegAppName( acdbHostApplicationServices()->workingDatabase(), DRAW_PARAMS_XDATA_GROUP );

        // 初始化所有的图元类型的当前可视化效果
        InitAllCurDraws();

        // 添加文档相关的反应器
        ReactorHelper::AddDocumentReactor( curDoc() );

        acutPrintf( _T( "\nMineGE::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        // 删除文档相关的反应器
        ReactorHelper::RemoveDocumentReactor( curDoc() );

        acutPrintf( _T( "\nMineGE::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    /*static void MineGE_movedata(void)
    {
    	extern void MoveData();

    	MoveData();
    }*/
} ;

IMPLEMENT_ARX_ENTRYPOINT( CMineGEApp )
//ACED_ARXCOMMAND_ENTRY_AUTO(CMineGEApp, MineGE,  _movedata, movedata, ACRX_CMD_TRANSPARENT, NULL)
