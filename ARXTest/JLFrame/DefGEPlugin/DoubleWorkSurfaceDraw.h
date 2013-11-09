#pragma once

#include "DoubleTunnelDraw.h"

// �򵥵�˫����Ӱ������
class DoubleWorkSurfaceDraw : public DoubleTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleWorkSurfaceDraw);

public:
	DoubleWorkSurfaceDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	void drawArrow(AcGiWorldDraw *mode, bool clockWise);
	void drawText(AcGiWorldDraw *mode);

	// Ĭ��ΪAcDb::kLnWt060
	//AcDb::LineWeight m_oneSideLineWeight;
	bool m_clockWise;
	double m_trunkWidth;   // ���ɰ��
	double m_trunkLength;  // ���ɳ���
	double m_arrowWidth;   // ��ͷ������Ĳ���
	double m_arrowLength;  // ��ͷ����
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleWorkSurfaceDraw)
#endif