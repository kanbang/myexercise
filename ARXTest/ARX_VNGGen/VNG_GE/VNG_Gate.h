#pragma once

#include "VNG_GE.h"

// ͨ������ͼ--������
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
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;          // �Ƕ�(��ʱ�룩

	// extra param
	double m_radius;        // ���ŵİ뾶
	double m_length;        // ����ֱ�ߵ�һ�볤�ȣ�����������ľ��룩
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Gate)
#endif
