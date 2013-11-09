#include "StdAfx.h"
#include "ReactorHelper.h"

#include "PointMonitor.h"

#include <map>
struct Contour_DocumentReactor
{
    Contour_DocumentReactor() : pPointMonitor( 0 ) {}
    PointMonitor* pPointMonitor;

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
typedef std::map<long, Contour_DocumentReactor> DocumentReactorMap;

DocumentReactorMap* pDocumentReactorMap_Contour = 0;

void ReactorHelper::CreateDocumentReactorMap()
{
    if( pDocumentReactorMap_Contour == 0 )
    {
        pDocumentReactorMap_Contour = new DocumentReactorMap();
    }
}

void ReactorHelper::RemoveDocumentReactorMap()
{
    delete pDocumentReactorMap_Contour;
    pDocumentReactorMap_Contour = 0;
}

void ReactorHelper::AddDocumentReactor( AcApDocument* pDoc )
{
    if( pDoc == 0 ) return;
    if( pDocumentReactorMap_Contour == 0 ) return;
    long id = ( long )pDoc;
    DocumentReactorMap::iterator itr = pDocumentReactorMap_Contour->find( id );
    if( itr != pDocumentReactorMap_Contour->end() ) return;

    Contour_DocumentReactor dr;
    dr.pPointMonitor = new PointMonitor( pDoc ); // 鼠标监视器
    pDocumentReactorMap_Contour->insert( DocumentReactorMap::value_type( id, dr ) );
}

void ReactorHelper::RemoveDocumentReactor( AcApDocument* pDoc )
{
    if( pDoc == 0 ) return;
    if( pDocumentReactorMap_Contour == 0 ) return;
    long id = ( long )pDoc;
    DocumentReactorMap::iterator itr = pDocumentReactorMap_Contour->find( id );
    if( itr == pDocumentReactorMap_Contour->end() ) return;

    // 解除绑定
    Contour_DocumentReactor dr = itr->second;
    delete dr.pPointMonitor;

    pDocumentReactorMap_Contour->erase( id );
}