#pragma once

#include "acssgetfilter.h"

// ����ѡ���еĵ�ֵ��ͼԪ
// �κ�ѡ����صĲ������޷�ѡ��ͼԪ
// �������ľ����û��޷��ֶ�ɾ����ֵ�ߣ�ֻ��ͨ���ṩ������ɾ����
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
