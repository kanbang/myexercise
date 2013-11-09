#include "StdAfx.h"
#include "ReactorHelper.h"

#include "PointMonitor.h"

#include <map>
struct Contour_DocumentReactor
{
    Contour_DocumentReactor() : pPointMonitor( 0 ) {}
    PointMonitor* pPointMonitor;

    // ��������������� AcDbDatabaseReactor��
    // AcEdInputPointMonitor������ApApDocument����reactor
    // ...
    // ������һ����ڹ���Ҳ����˶��ĵ�������
    // 1) �½��ĵ�������kLoadDwgMsg��Ϣ
    // 2) �����ĵ�������arx����Ҳ�������е��ĵ�����kLoadDwgMsg��Ϣ
    // 3) �ر��ĵ��� ����kUnloadDwgMsg��Ϣ
    // 4) ж��arx����Ҳ�������е��ĵ�����kUnloadDwgMsg��Ϣ
};

/*
 * long���ͱ�ʾAcApDocument*��ָ���ַ
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
    dr.pPointMonitor = new PointMonitor( pDoc ); // ��������
    pDocumentReactorMap_Contour->insert( DocumentReactorMap::value_type( id, dr ) );
}

void ReactorHelper::RemoveDocumentReactor( AcApDocument* pDoc )
{
    if( pDoc == 0 ) return;
    if( pDocumentReactorMap_Contour == 0 ) return;
    long id = ( long )pDoc;
    DocumentReactorMap::iterator itr = pDocumentReactorMap_Contour->find( id );
    if( itr == pDocumentReactorMap_Contour->end() ) return;

    // �����
    Contour_DocumentReactor dr = itr->second;
    delete dr.pPointMonitor;

    pDocumentReactorMap_Contour->erase( id );
}