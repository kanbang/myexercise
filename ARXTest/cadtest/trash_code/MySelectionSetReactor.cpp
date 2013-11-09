#include "StdAfx.h"
#include "MySelectionSetReactor.h"

MySelectionSetReactor::MySelectionSetReactor ( AcApDocument* pDoc ) : AcEdSSGetFilter ()
{
    mpDoc = NULL ;
    if ( pDoc != NULL )
    {
        mpDoc = pDoc ;
        addSSgetFilterInputContextReactor ( mpDoc, this ) ;
    }
}

MySelectionSetReactor::~MySelectionSetReactor ()
{
    Detach () ;
}

void MySelectionSetReactor::Attach ()
{
    Detach () ;
    mpDoc = acDocManager->curDocument () ;
    if ( mpDoc )
        addSSgetFilterInputContextReactor ( mpDoc, this ) ;
}

void MySelectionSetReactor::Detach ()
{
    if ( mpDoc )
    {
        removeSSgetFilterInputContextReactor ( mpDoc, this ) ;
        mpDoc = NULL ;
    }
}

void MySelectionSetReactor::ssgetAddFilter ( int ssgetFlags, AcEdSelectionSetService& service,
        const AcDbObjectIdArray& selectionSet,
        const AcDbObjectIdArray& subSelectionSet )
{
    acutPrintf( _T( "\nѡ���ͼԪ������%d" ), selectionSet.length() );
}
//
//// -----------------------------------------------------------------------------
//void MySelectionSetReactor::endSSGet(Acad::PromptStatus returnStatus, int ssgetFlags, AcEdSelectionSetService & service, const AcDbObjectIdArray & selectionSet)
//{
//	acutPrintf(_T("\n����ѡ���ͼԪ������%d"), selectionSet.length());
//
//	AcEdSSGetFilter::endSSGet (returnStatus, ssgetFlags, service, selectionSet) ;
//}
