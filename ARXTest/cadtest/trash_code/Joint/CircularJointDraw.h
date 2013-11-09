#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class CircularJointDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(CircularJointDraw);

public:
	CircularJointDraw();
	~CircularJointDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

private:
	void drawLine(AcGiWorldDraw *mode);
	void drawCircle(AcGiWorldDraw *mode);

	// Ȩ��Ϊpublic�������
public:
	// key param
	AcGePoint3d m_insertPt;

	// extra param
	double m_radius;  // Բ�뾶(Ĭ��Ϊ30)
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CircularJointDraw)
#endif