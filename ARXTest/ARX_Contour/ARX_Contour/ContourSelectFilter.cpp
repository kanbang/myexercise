#include "StdAfx.h"
#include "ContourSelectFilter.h"

#include "../ContourGE/ContourGE.h"
#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

ContourSelectFilter::ContourSelectFilter ( AcApDocument* pDoc )
    : AcEdSSGetFilter ()
{
    mpDoc = NULL ;
    if ( pDoc != NULL )
    {
        mpDoc = pDoc ;
        addSSgetFilterInputContextReactor ( mpDoc, this ) ;
    }
}

ContourSelectFilter::~ContourSelectFilter ()
{
    Detach () ;
}

void ContourSelectFilter::Attach ()
{
    Detach () ;
    mpDoc = acDocManager->curDocument () ;
    if ( mpDoc )
        addSSgetFilterInputContextReactor ( mpDoc, this ) ;
}

void ContourSelectFilter::Detach ()
{
    if ( mpDoc )
    {
        removeSSgetFilterInputContextReactor ( mpDoc, this ) ;
        mpDoc = NULL ;
    }
}

void ContourSelectFilter::ssgetAddFilter (
    int ssgetFlags,
    AcEdSelectionSetService& service,
    const AcDbObjectIdArray& selectionSet,
    const AcDbObjectIdArray& subSelectionSet
)
{
    //acutPrintf(_T("\n ssgetAddFilter==> select set: %d"), selectionSet.length());
    //acutPrintf(_T("\n ssgetAddFilter==> subselect set: %d\n"), subSelectionSet.length());

    AcGeIntArray ia;
    int n = subSelectionSet.length();
    for( int i = 0; i < n; i++ )
    {

        if( ArxUtilHelper::IsEqualType( _T( "ContourGE" ), subSelectionSet[i] ) )
        {
            ia.append( i );
        }
    }

    n = ia.length();
    for( int i = 0; i < n; i++ )
    {
        service.remove( ia[i] );
    }
}

void ContourSelectFilter::endSSGet(
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
