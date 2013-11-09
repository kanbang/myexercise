#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// 挡沙土墙默认效果
class SimpleSandWallDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleSandWallDraw) ;

public:
	SimpleSandWallDraw () ;
	virtual ~SimpleSandWallDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();	
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;

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

protected:
	// key param
	AcGePoint3d m_pt;  // 插入点坐标
	double m_angle;    // 角度(逆时针）
	double m_length;   // 墙的长度

	// extra param
	double m_thick;   // 矩形块宽度
	bool needFill;    // 是否填充
	double m_scale;   // 填充比例
} ;

#ifdef FFC_GEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleSandWallDraw)
#endif
