#pragma once

#include "../MineGE/LinkedGEDraw.h"

// 单折线巷道(默认具有1个折点)
class SingleBlendTunnelDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SingleBlendTunnelDraw);

public:
	SingleBlendTunnelDraw(void);
	~SingleBlendTunnelDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	// 变换操作(移动、选择、镜像)--目前暂不考虑"镜像"操作
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	//- Osnap points protocol
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	void drawLine(AcGiWorldDraw* mode, const AcGePoint3d& startPt, const AcGePoint3d& endPt);

	// extra param
	AcGePoint3dArray m_blendPts; // 折点集合
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SingleBlendTunnelDraw)
#endif