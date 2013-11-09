#pragma once

#include "acssgetfilter.h"

class OnlyOneSelectFilter : public AcEdSSGetFilter {

protected:
	AcApDocument *mpDoc ;

public:
	OnlyOneSelectFilter (AcApDocument *pDoc =acDocManager->curDocument ()) ;
	virtual ~OnlyOneSelectFilter () ;

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

	virtual void endSSGet(
		Acad::PromptStatus returnStatus, 
		int ssgetFlags, 
		AcEdSelectionSetService & service, 
		const AcDbObjectIdArray & selectionSet);
} ;
