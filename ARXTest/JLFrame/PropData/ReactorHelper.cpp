#include "StdAfx.h"
#include "ReactorHelper.h"

#include "PropDataTooltipMonitor.h"
//#include "LinkedGE_EditorReactor.h"
//#include "MineGEErase_DbReactor.h"

/*
 * 用于记录与Document相关的reactor
 * 例如AcDbDataBaseReactor、AcEdInputPointMonitor等都是与文档相关的
 * 一个文档对象对应一个reactor，可以看做是"局部的"
 * 而AcEditorReactor属于AcEditor，可以看做是"全局的"
 * 从arx的消息流程可以看出，在kLoadAppMsg()中AcEditor有效，
 * 而其他的例如文档对象(AcApDocument)、数据库对象(AcDbDatabase)还是无效的
 * 只有在kLoadDwgMsg()中的文档和数据库对象才开始有效
 */
#include <map>
struct PropData_DocumentReactor
{
    PropData_DocumentReactor() : pTooltipMonitor( 0 ) {}
    PropDataTooltipMonitor* pTooltipMonitor;
    //MineGEErase_DbReactor* pEraseDbReactor;
    //LinkedGE_EditorReactor* pLinkedGEDatabaseReactor;

    // 还可以添加其它的 AcDbDatabaseReactor、
    // AcEdInputPointMonitor等属于ApApDocument类别的reactor
    // ...
    // 集中在一起便于管理，也解决了多文档的问题
    // 1) 新建文档，发送kLoadDwgMsg消息
    // 2) 已有文档，加载arx程序，也会向所有的文档发送kLoadDwgMsg消息
    // 3) 关闭文档， 发送kUnloadDwgMsg消息
    // 4) 卸载arx程序，也会向所有的文档发送kUnloadDwgMsg消息
};

/*
 * long类型表示AcApDocument*的指针地址
 */
typedef std::map<long, PropData_DocumentReactor> DocumentReactorMap;

DocumentReactorMap* pDocumentReactorMap_PropData = 0;

void ReactorHelper::CreateDocumentReactorMap()
{
    if( pDocumentReactorMap_PropData == 0 )
    {
        pDocumentReactorMap_PropData = new DocumentReactorMap();
    }
}

void ReactorHelper::RemoveDocumentReactorMap()
{
    delete pDocumentReactorMap_PropData;
    pDocumentReactorMap_PropData = 0;
}

void ReactorHelper::AddDocumentReactor( AcApDocument* pDoc )
{
    if( pDoc == 0 ) return;
    if( pDocumentReactorMap_PropData == 0 ) return;
    long id = ( long )pDoc;
    DocumentReactorMap::iterator itr = pDocumentReactorMap_PropData->find( id );
    if( itr != pDocumentReactorMap_PropData->end() ) return;

    PropData_DocumentReactor dr;
    dr.pTooltipMonitor = new PropDataTooltipMonitor( pDoc );
    //dr.pEraseDbReactor = new MineGEErase_DbReactor(pDoc->database());
    //dr.pLinkedGEDatabaseReactor = new LinkedGEAppendReactor(pDoc->database());

    pDocumentReactorMap_PropData->insert( DocumentReactorMap::value_type( id, dr ) );
}

void ReactorHelper::RemoveDocumentReactor( AcApDocument* pDoc )
{
    if( pDoc == 0 ) return;
    if( pDocumentReactorMap_PropData == 0 ) return;
    long id = ( long )pDoc;
    DocumentReactorMap::iterator itr = pDocumentReactorMap_PropData->find( id );
    if( itr == pDocumentReactorMap_PropData->end() ) return;

    // 解除绑定
    PropData_DocumentReactor dr = itr->second;
    delete dr.pTooltipMonitor;
    //delete dr.pEraseDbReactor;
    //delete dr.pLinkedGEDatabaseReactor;

    pDocumentReactorMap_PropData->erase( id );
}