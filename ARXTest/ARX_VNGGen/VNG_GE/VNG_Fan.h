#pragma once

#include "VNG_GE.h"

// ͨ������ͼ--ͨ�綯��
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
	AcGePoint3d m_insertPt; // ���������
	double m_angle;         // �Ƕ�

	// extra param
	double m_radius;       // �뾶
	double m_distance;     // ����
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Fan)
#endif
