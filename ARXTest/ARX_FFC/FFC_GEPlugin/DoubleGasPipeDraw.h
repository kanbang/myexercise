#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class DoubleGasPipeDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleGasPipeDraw);

public:
	DoubleGasPipeDraw();

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();	

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	void update();
	void caclLeftPoint(double angle, AcGePoint3d& startPt, AcGePoint3d& endPt);
	void caclRightPoint(double angle, AcGePoint3d& startPt, AcGePoint3d& endPt);

private:
	// key param
	AcGePoint3d m_startPt, m_endPt;   // 始末点坐标

	// extra param
	double m_width;
	AcGePoint3d m_leftStartPt, m_leftEndPt;
	AcGePoint3d m_rightStartPt, m_rightEndPt;
	double m_radius;    // 球体半径
};

#ifdef FFC_GEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleGasPipeDraw)
#endif