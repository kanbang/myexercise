#pragma once

#include "DoubleTunnelDraw.h"

// 简单井筒绘制效果
// 直接利用已有的双线巷道绘制效果
class DoubleShaftDraw : public DoubleTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleShaftDraw);

public:
	DoubleShaftDraw();

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	AcDb::LineWeight m_lineWeight;
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleShaftDraw)
#endif