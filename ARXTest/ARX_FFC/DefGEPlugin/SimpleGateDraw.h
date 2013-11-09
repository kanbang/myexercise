#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// ���ſ��ӻ�Ĭ��Ч��
class SimpleGateDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleGateDraw) ;

public:
	SimpleGateDraw () ;
	virtual ~SimpleGateDraw () ;

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
	void drawArc(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle, double gap, double radius, bool moreAngle = true);
	void drawLine(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle,  double gap, double offset, double length);
	
protected:
	// key param
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;         // �Ƕ�(��ʱ�룩

	// extra param
	double m_radius;        // ���ŵİ뾶
	double m_length;        // ����ֱ�ߵ�һ�볤�ȣ�����������ľ��룩

	// �����������Ҫ��ӵ����Ӳ�����
	double m_offset;
	double m_gap;

	double m_dTWidth;  // ����˫������Ŀ��

} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleGateDraw)
#endif
