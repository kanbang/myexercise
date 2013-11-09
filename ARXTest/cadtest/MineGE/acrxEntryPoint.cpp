#include "StdAfx.h"

#include "config.h"
#include "HelperClass.h"
#include "ReactorHelper.h"
#include "CurDrawTool.h"

#include "../ArxHelper/HelperClass.h"

// ����ע���������
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

        // ע�����
        acrxRegisterService( MINEGE_SERVICE_NAME );

        ReactorHelper::CreateLinkedGE_EditorReactor();
        ReactorHelper::CreateDocumentReactorMap();

        acutPrintf( _T( "\nMineGE::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // ɾ��AcEditorReactor
        ReactorHelper::RemoveLinkedGE_EditorReactor();
        ReactorHelper::RemoveDocumentReactorMap();

        // ɾ������
        delete acrxServiceDictionary->remove( MINEGE_SERVICE_NAME );

        acutPrintf( _T( "\nMineGE::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    static void PrintAddress()
    {
        AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
        AcEditor* pEditor = acedEditor;
        AcApDocument* pDoc = curDoc();
        AcDbDatabase* pDocDB = pDoc->database();
        AcEdInputPointManager* pIPM = pDoc->inputPointManager();

        acutPrintf( _T( "\nworking database: %ld" ), ( long )pDB );
        acutPrintf( _T( "\neditor: %ld" ), ( long )pEditor );
        acutPrintf( _T( "\ncurrent doc: %ld" ), ( long )pDoc );
        acutPrintf( _T( "\ncurrent doc database: %ld" ), ( long )pDocDB );
        acutPrintf( _T( "\ncurrent doc InputPointManager: %ld" ), ( long )pIPM );
    }

    // ���������������ĵ�����AcDocument�������reactor
    //  ����AcEdInputPointMonitor��
    // ��On_kLoadDwgMsg��On_kUnloadDwgMsg��Ϣ���ĵ���������Ч�ģ�
    // ����On_kInitAppMsg��On_kUnloadAppMsg
    // �ĵ������Ѿ�ʧЧ; ���ݿ����ͬ��Ҳ�����
    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // ע��dictionary��regAppName
        ArxDataTool::RegDict( PROPERTY_DATA_FIELD_DICT );
        ArxDataTool::RegDict( PROPERTY_DATA_FIELD_INFO_DICT );
        ArxDataTool::RegDict( TYPE_NAME_MAP_DICT );
        ArxDataTool::RegDict( STRING_LIST_DICT );
        ArxDataTool::RegDict( INT_LIST_DICT );
        ArxDataTool::RegDict( OBJECT_LIST_DICT );
        ArxDataTool::RegDict( OBJECT_LIST_DATA_DICT );
        ArxDataTool::RegDict( GLOBAL_SINGLE_INFO_DICT );
        ArxDataTool::RegDict( CUR_DRAW_DICT );

        ArxDataTool::RegAppName( acdbHostApplicationServices()->workingDatabase(), DRAW_PARAMS_XDATA_GROUP );

        //PrintAddress();

        ReactorHelper::AddDocumentReactor( curDoc() );

        // ��ʼ�����е�ͼԪ���͵ĵ�ǰ���ӻ�Ч��
        // Ŀǰ�Ĵ���ʵ��ֻ��������ӵ�ͼԪ����
        InitAllCurDraws();

        acutPrintf( _T( "\nMineGE::On_kLoadDwgMsg\n" ) );
        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        ReactorHelper::RemoveDocumentReactor( curDoc() );

        acutPrintf( _T( "\nMineGE::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void MineGE_ZoomTest( void )
    {
        AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��ͼԪʵ��:" ) );
        if( objId.isNull() ) return;

        ArxEntityHelper::ZoomToEntity( objId );
    }

    /*static void MineGE_movedata(void)
    {
    	extern void MoveData();

    	MoveData();
    }*/
} ;

IMPLEMENT_ARX_ENTRYPOINT( CMineGEApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CMineGEApp, MineGE,  _ZoomTest, ZoomTest, ACRX_CMD_TRANSPARENT, NULL )
//ACED_ARXCOMMAND_ENTRY_AUTO(CMineGEApp, MineGE,  _movedata, movedata, ACRX_CMD_TRANSPARENT, NULL)
