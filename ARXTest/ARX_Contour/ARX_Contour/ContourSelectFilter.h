#pragma once

#include "acssgetfilter.h"

// 过滤选择集中的等值线图元
// 任何选择相关的操作都无法选中图元
// 【隐含的就是用户无法手动删除等值线，只能通过提供的命令删除】
class ContourSelectFilter : public AcEdSSGetFilter {

protected:
	AcApDocument *mpDoc ;

public:
	ContourSelectFilter (AcApDocument *pDoc =acDocManager->curDocument ()) ;
	virtual ~ContourSelectFilter () ;

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
