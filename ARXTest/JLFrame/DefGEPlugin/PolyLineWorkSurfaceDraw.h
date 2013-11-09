#pragma once

#include "PolyLineTunnelDraw.h"

class PolyLineWorkSurfaceDraw : public PolyLineTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineWorkSurfaceDraw);

public:
	PolyLineWorkSurfaceDraw(void);

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

	bool m_clockWise;
	double m_trunkWidth;   // ���ɰ��
	double m_trunkLength;  // ���ɳ���
	double m_arrowWidth;   // ��ͷ������Ĳ���
	double m_arrowLength;  // ��ͷ����
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineWorkSurfaceDraw)
#endif