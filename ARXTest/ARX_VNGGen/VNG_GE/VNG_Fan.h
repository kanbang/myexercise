#pragma once

#include "VNG_GE.h"

// 通风网络图--通风动力
class VNG_GE_DLLIMPEXP VNG_Fan : public VNG_GE {

public:
	ACRX_DECLARE_MEMBERS(VNG_Fan) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Fan () ;
	virtual ~VNG_Fan () ;

	void setData(const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept);
	void setParam(double m_radius, double m_distance);
	
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

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

	virtual Acad::ErrorStatus subGetGripPoints( 
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

public:
	// key param
	AcGePoint3d m_insertPt; // 插入点坐标
	double m_angle;         // 角度

	// extra param
	double m_radius;       // 半径
	double m_distance;     // 距离
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Fan)
#endif
