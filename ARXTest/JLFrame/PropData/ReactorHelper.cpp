#include "StdAfx.h"
#include "ReactorHelper.h"

#include "PropDataTooltipMonitor.h"
//#include "LinkedGE_EditorReactor.h"
//#include "MineGEErase_DbReactor.h"

/*
 * ���ڼ�¼��Document��ص�reactor
 * ����AcDbDataBaseReactor��AcEdInputPointMonitor�ȶ������ĵ���ص�
 * һ���ĵ������Ӧһ��reactor�����Կ�����"�ֲ���"
 * ��AcEditorReactor����AcEditor�����Կ�����"ȫ�ֵ�"
 * ��arx����Ϣ���̿��Կ�������kLoadAppMsg()��AcEditor��Ч��
 * �������������ĵ�����(AcApDocument)�����ݿ����(AcDbDatabase)������Ч��
 * ֻ����kLoadDwgMsg()�е��ĵ������ݿ����ſ�ʼ��Ч
 */
#include <map>
struct PropData_DocumentReactor
{
    PropData_DocumentReactor() : pTooltipMonitor( 0 ) {}
    PropDataTooltipMonitor* pTooltipMonitor;
    //MineGEErase_DbReactor* pEraseDbReactor;
    //LinkedGE_EditorReactor* pLinkedGEDatabaseReactor;

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

    // �����
    PropData_DocumentReactor dr = itr->second;
    delete dr.pTooltipMonitor;
    //delete dr.pEraseDbReactor;
    //delete dr.pLinkedGEDatabaseReactor;

    pDocumentReactorMap_PropData->erase( id );
}