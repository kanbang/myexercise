#pragma once

#include "dbmain.h"

class DLJJoint : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(DLJJoint) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DLJJoint () ;
	virtual ~DLJJoint () ;

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//virtual AcRxObject* clone() const;

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	//virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	//virtual Acad::ErrorStatus subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const;
	//virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const;
	//virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset);

	// 为了测试方便，参数的访问权限为public
	// 在实际开发中，参数的访问权限一般为private/protected
public:
	AcGePoint3d m_pt;              // 圆心坐标
	double m_radius;               // 圆心半径
} ;

#ifdef HELLOWORLD_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DLJJoint)
#endif
