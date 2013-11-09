#pragma once

#include "VNG_GE.h"

// 通风网络图--构筑物
class VNG_GE_DLLIMPEXP VNG_Gate : public VNG_GE {

public:
	ACRX_DECLARE_MEMBERS(VNG_Gate) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Gate () ;
	virtual ~VNG_Gate () ;

	void setData(const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept);
	void setParam(double m_radius, double m_length);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

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

public:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 角度(逆时针）

	// extra param
	double m_radius;        // 风门的半径
	double m_length;        // 风门直线的一半长度（从中轴向外的距离）
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Gate)
#endif
