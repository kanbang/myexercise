#pragma once

#include "acedinpt.h"

/*
 * ע�����е�Reactor���Բ�ʹ��arx��
 * �����޸������ͣ��ͼԪ�Ϸ���ʱ�򵯳���tooltip
 * �������Ǻ�AcDocument�����ģ������Ҫ��On_kLoadDwgMsg 
 * �� On_kUnloadDwgMsg��Ϣ�еĳ�ʼ����ж��
 */
class MineGETooltipMonitor : public AcEdInputPointMonitor 
{
protected:
	AcApDocument *mpDocument ;

public:
	MineGETooltipMonitor (AcApDocument *pDoc =NULL) ;
	virtual ~MineGETooltipMonitor () ;

	virtual void Attach (AcApDocument *pDoc) ;
	virtual void Detach () ;
	virtual AcApDocument *Subject () const ;
	virtual bool IsAttached () const ;

	virtual bool excludeFromOsnapCalculation (
		const AcArray<AcDbObjectId> &nestedEntity,
		Adesk::GsMarker gsSelectionMark
	) ;

	virtual Acad::ErrorStatus monitorInputPoint (
		bool &appendToTooltipStr,
		ACHAR *&additionalTooltipString,
		AcGiViewportDraw *drawContext,
		AcApDocument *document,
		bool pointComputed,
		int history,
		const AcGePoint3d &lastPoint,
		const AcGePoint3d &rawPoint,
		const AcGePoint3d &grippedPoint,
		const AcGePoint3d &cartesianSnappedPoint,
		const AcGePoint3d &osnappedPoint,
		AcDb::OsnapMask osnapMask,
		const AcArray<AcDbCustomOsnapMode *> &customOsnapModes,
		AcDb::OsnapMask osnapOverrides,
		const AcArray<AcDbCustomOsnapMode *> &customOsnapOverrides,
		const AcArray<AcDbObjectId> &apertureEntities,
		const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> > &nestedApertureEntities,
		const AcArray<Adesk::GsMarker> &gsSelectionMark,
		const AcArray<AcDbObjectId> &keyPointEntities,
		const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> > &nestedKeyPointEntities,
		const AcArray<Adesk::GsMarker> &keyPointGsSelectionMark,
		const AcArray<AcGeCurve3d *> &alignmentPaths,
		const AcGePoint3d &computedPoint,
		const ACHAR *tooltipString
	) ;
} ;
