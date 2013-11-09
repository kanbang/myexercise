#include "StdAfx.h"
#include "OnlyOneSelectFilter.h"

OnlyOneSelectFilter::OnlyOneSelectFilter ( AcApDocument* pDoc )
    : AcEdSSGetFilter ()
{
    mpDoc = NULL ;
    if ( pDoc != NULL )
    {
        mpDoc = pDoc ;
        addSSgetFilterInputContextReactor ( mpDoc, this ) ;
    }
}

OnlyOneSelectFilter::~OnlyOneSelectFilter ()
{
    Detach () ;
}

void OnlyOneSelectFilter::Attach ()
{
    Detach () ;
    mpDoc = acDocManager->curDocument () ;
    if ( mpDoc )
        addSSgetFilterInputContextReactor ( mpDoc, this ) ;
}

void OnlyOneSelectFilter::Detach ()
{
    if ( mpDoc )
    {
        removeSSgetFilterInputContextReactor ( mpDoc, this ) ;
        mpDoc = NULL ;
    }
}

void OnlyOneSelectFilter::ssgetAddFilter (
    int ssgetFlags,
    AcEdSelectionSetService& service,
    const AcDbObjectIdArray& selectionSet,
    const AcDbObjectIdArray& subSelectionSet
)
{
    //acutPrintf(_T("\n ssgetAddFilter==> select set: %d"), selectionSet.length());
    //acutPrintf(_T("\n ssgetAddFilter==> subselect set: %d\n"), subSelectionSet.length());

    //AcEdSSGetFilter::ssgetAddFilter (ssgetFlags, service, selectionSet, subSelectionSet) ;
    if( selectionSet.length() == 0 )
    {
        if( subSelectionSet.length() > 1 )
        {
            int len = subSelectionSet.length();
            for( int i = 0; i < len - 1; i++ )
            {
                Acad::ErrorStatus es = service.remove( i );
            }
        }
    }
    else
    {
        int len = subSelectionSet.length();
        for( int i = 0; i < len; i++ )
        {
            Acad::ErrorStatus es = service.remove( i );
        }
    }
}

void OnlyOneSelectFilter::endSSGet(
    Acad::PromptStatus returnStatus,
    int ssgetFlags,
    AcEdSelectionSetService& service,
    const AcDbObjectIdArray& selectionSet )
{
    //acutPrintf(_T("\n endSSGet==> select set: %d\n"), selectionSet.length());

    //AcEdSSGetFilter::endSSGet (returnStatus, ssgetFlags, service, selectionSet) ;
    // ��endSSGet��ʹ��remove�������Խ�ʵ��(entity)��ѡ��(selectionSet)���ų�
    // ���ڲ��Ե�ʱ���֣����ų���ʵ��ļе���Ȼ��ʾ������(��֪��ʲôԭ��)

    //int len = selectionSet.length();
    //for(int i=0;i<len-1;i++)
    //{
    //	service.remove(i);
    //}
}
