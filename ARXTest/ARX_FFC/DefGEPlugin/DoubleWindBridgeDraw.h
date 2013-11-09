#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// ˫�߷��ſ��ӻ�Ч��
class DoubleWindBridgeDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleWindBridgeDraw);

public:
	DoubleWindBridgeDraw();

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

	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	// key param
	AcGePoint3d m_insertPt;   // ���������
	double m_angle;           // ����Ƕȣ���ʱ�룬��λ���ȣ�

	// ����˫������ĸ��Ӳ���֮��
	// �������ӻ��Ʒ�����Ҫ����������
	double m_width;
	double m_sideLength;  // ��߳���
	double m_sideLength2; // ����ǵĲ�߳���
	double m_sideAngle;   // ������(�Բ��Ϊ��׼����)
	double m_sideOffset;  // ���ƫ��(�����������Ϊ��׼)
	double m_linewidth; // �߿�

public:
	AcGePoint3d caclSidePt(const AcGePoint3d& pt, const AcGeVector3d& baseV, bool clockwise) const;
	void drawSide(AcGiWorldDraw *mode);
	void drawOneSide(AcGiWorldDraw *mode, const AcGeVector3d& v, const AcGePoint3d& pt, bool clockwise);
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleWindBridgeDraw)
#endif