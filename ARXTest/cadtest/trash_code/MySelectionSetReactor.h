#pragma once

#include "acssgetfilter.h"

class MySelectionSetReactor : public AcEdSSGetFilter {

protected:
	AcApDocument *mpDoc ;

public:
	MySelectionSetReactor (AcApDocument *pDoc =acDocManager->curDocument ()) ;
	virtual ~MySelectionSetReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcApDocument *Subject () const { return (mpDoc) ; }
	virtual bool IsAttached () const { return (mpDoc != NULL) ; }

	virtual void ssgetAddFilter (
		int ssgetFlags,
		AcEdSelectionSetService &service,
		const AcDbObjectIdArray &selectionSet,
		const AcDbObjectIdArray &subSelectionSet
	) ;

	// -----------------------------------------------------------------------------
	//virtual void endSSGet(Acad::PromptStatus returnStatus, int ssgetFlags, AcEdSelectionSetService & service, const AcDbObjectIdArray & selectionSet);
} ;
