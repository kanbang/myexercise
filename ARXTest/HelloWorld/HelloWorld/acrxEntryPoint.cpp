#include "StdAfx.h"
#include "resource.h"

#include "DLJJoint.h"
//#include "OnlyOneSelectFilter.h"
//#include "SelectedGE_EditorReactor.h"

//OnlyOneSelectFilter* pOnlyOneSelectFilter;
//SelectedGE_EditorReactor* pSelectedGE_EditorReactor;
/* 切换开关(全局变量) */
bool SSGetFilterSwitch;

#include "TestDlg.h"
#include "TestDlg2.h"
#include "TestDlg3.h"
#include "TestDlg4.h"
#include "TestDlg5.h"
#include "HelpInfoDlg.h"

#include "MyDbReactor.h"

MyDbReactor* pDBReactor = 0;
class CHelloWorldApp : public AcRxArxApp
{

public:
    CHelloWorldApp() : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        // TODO: Load dependencies here

        // You *must* call On_kInitAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        //pSelectedGE_EditorReactor = new SelectedGE_EditorReactor();


        acutPrintf( _T( "\nHelloWorld::On_kInitAppMsg()...\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        // TODO: Add your code here

        // You *must* call On_kUnloadAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        //delete pSelectedGE_EditorReactor;
        //pSelectedGE_EditorReactor = 0;

        acutPrintf( _T( "\nHelloWorld::On_kUnloadAppMsg()...\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        //pOnlyOneSelectFilter = new OnlyOneSelectFilter(curDoc());

        //pDBReactor = new MyDbReactor(curDoc()->database());

        acutPrintf( _T( "\nMineGE::On_kLoadDwgMsg\n" ) );
        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        //delete pOnlyOneSelectFilter;
        //pOnlyOneSelectFilter = 0;
        //delete pDBReactor;
        //pDBReactor = 0;

        acutPrintf( _T( "\nMineGE::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }


    // - HelloWorld._test command (do not rename)
    static void HelloWorld_test( void )
    {
        acutPrintf( _T( "HelloWorld" ) );
        acutPrintf( _T( "\n%d   %s   %.5f" ), 10, _T( "aa" ), 13.4 );
        acutPrintf( _T( "===\t\t\n" ) );


        //struct resbuf eb1, eb2, eb3;
        ////acutBuildList(0, _T("LINE"), 0);
        //ACHAR sbuf1[10], sbuf2[10]; // Buffers to hold strings
        //eb1.restype = 0;	// Entity name
        //_tcscpy(sbuf1, _T("LINE"));
        //eb1.resval.rstring = sbuf1;
        //eb1.rbnext = NULL; // No other properties

        //ads_name ssname;
        //acedSSGet(NULL, NULL, NULL, &eb1, ssname);

        ads_name ename;
        ads_point pt;
        acedEntSel( NULL, ename, pt );
        AcDbObjectId objId;
        acdbGetObjectId( objId, ename );

        AcDbEntity* pEnt;
        acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
        pEnt->highlight();
        pEnt->close();
    }

    static void HelloWorld_test2( void )
    {
        AcDbBlockTable* pBT;
        AcDbBlockTableRecord* pBTR;
        acdbHostApplicationServices()->workingDatabase()
        ->getBlockTable( pBT, AcDb::kForRead );
        pBT->getAt( ACDB_MODEL_SPACE, pBTR, AcDb::kForRead );

        acutPrintf( _T( "\nbtr:%s" ), pBTR->isA()->name() );
        pBT->close();

        AcDbBlockTableRecordIterator* pBTRI;
        pBTR->newIterator( pBTRI );
        for( pBTRI->start(); ! pBTRI->done(); pBTRI->step() )
        {
            AcDbEntity* pEnt;
            if( Acad::eOk == pBTRI->getEntity( pEnt, AcDb::kForWrite ) )
            {
                pEnt->erase();
                pEnt->close();
            }
        }
        delete pBTRI;
        pBTR->close();
    }

    static void PostEntToModelSpace( AcDbEntity* pEnt )
    {
        AcDbBlockTable* pBlockTable;
        acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable( pBlockTable, AcDb::kForRead );

        AcDbBlockTableRecord* pBlockTableRecord;
        pBlockTable->getAt( ACDB_MODEL_SPACE, pBlockTableRecord,
                            AcDb::kForWrite );
        pBlockTable->close();

        AcDbObjectId objId;
        pBlockTableRecord->appendAcDbEntity( objId, pEnt );

        pBlockTableRecord->close();
    }

    static void HelloWorld_DrawJoint( void )
    {
        acutPrintf( _T( "\n绘制一个十字圆..." ) );

        // 1) 创建实体对象，并初始化
        DLJJoint* pJoint = new DLJJoint();
        pJoint->m_pt = AcGePoint3d( 5000, 2000, 0 );
        pJoint->m_radius = 500;

        PostEntToModelSpace( pJoint );
        pJoint->close();
    }

    static void HelloWorld_SSGetSwitch( void )
    {
        resbuf rbPickAdd;
        acedGetVar( _T( "PICKADD" ), &rbPickAdd );
        short& iPickAdd = rbPickAdd.resval.rint;

        acutPrintf( _T( "\n系统变量==>PICKADD:%d" ), iPickAdd );

        iPickAdd = !iPickAdd;
        acedSetVar( _T( "PICKADD" ), &rbPickAdd );
        acutPrintf( _T( "\n修改狗系统变量==>PICKADD:%d" ), iPickAdd );
    }

    static void ExecuteZoomCommand2( const AcDbObjectId& objId )
    {
        ads_name en;
        if( Acad::eOk != acdbGetAdsName( en, objId ) ) return;
        acedCommand( RTSTR, _T( "ZOOM" ), RTSTR, _T( "O" ), RTENAME, en, RTSTR, _T( "" ), 0 );
    }

    static void HelloWorld_test3( void )
    {
        ads_name en;
        ads_point pt;
        if( RTNORM != acedEntSel( NULL, en, pt ) ) return;
        acutPrintf( _T( "\n选择实体成功!" ) );

        AcDbObjectId objId;
        if( Acad::eOk != acdbGetObjectId( objId, en ) ) return;

        //extern void ZoomToEntity( const AcDbObjectId& objId );

        ExecuteZoomCommand2( objId );
        //ZoomToEntity(objId);
    }

    static void HelloWorld_test4( void )
    {
        CAcModuleResourceOverride myResources;
        TestDlg dlg;
        dlg.DoModal();
    }

    static void HelloWorld_test5( void )
    {
        CAcModuleResourceOverride myResources;
        TestDlg2 dlg;
        dlg.DoModal();
    }

    static void HelloWorld_test6( void )
    {
        CAcModuleResourceOverride myResources;
        TestDlg3 dlg;
        dlg.DoModal();
    }

    static void HelloWorld_test7( void )
    {
        CAcModuleResourceOverride myResources;
        TestDlg4 dlg;
        dlg.DoModal();
    }

    static void HelloWorld_test8( void )
    {
        CAcModuleResourceOverride myResources;

        AfxEnableControlContainer();

        //InitCommonControls();

        TestDlg5 dlg;
        dlg.DoModal();
    }

    static void HelloWorld_test9( void )
    {
        CAcModuleResourceOverride myResources;

        HelpInfoDlg dlg;
        dlg.DoModal();
    }
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CHelloWorldApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test, test, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test2, test2, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test3, test3, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test4, test4, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test5, test5, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test6, test6, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test7, test7, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test8, test8, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _test9, test9, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _DrawJoint, DrawJoint, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CHelloWorldApp, HelloWorld, _SSGetSwitch, SSGetSwitch, ACRX_CMD_TRANSPARENT, NULL )
//ACED_ARXCOMMAND_ENTRY_AUTO(CHelloWorldApp, HelloWorld, _DrawArc, DrawArc, ACRX_CMD_TRANSPARENT, NULL)