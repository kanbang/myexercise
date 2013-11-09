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
    // 在endSSGet中使用remove方法可以将实体(entity)从选择集(selectionSet)中排除
    // 但在测试的时候发现，被排除的实体的夹点仍然显示出来了(不知道什么原因)

    //int len = selectionSet.length();
    //for(int i=0;i<len-1;i++)
    //{
    //	service.remove(i);
    //}
}
