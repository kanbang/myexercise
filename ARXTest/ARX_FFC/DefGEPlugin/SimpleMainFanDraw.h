#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// ��Ҫͨ����򵥿��ӻ�Ч��
class SimpleMainFanDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleMainFanDraw) ;

public:
	SimpleMainFanDraw () ;
	virtual ~SimpleMainFanDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

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
	double m_angle; // �Ƕ�

	// extra param
	double m_radius; // �뾶
	double m_distance; // ����
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleMainFanDraw)
#endif
