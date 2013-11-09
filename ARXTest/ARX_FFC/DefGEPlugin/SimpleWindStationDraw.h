#pragma once

#include "SimpleWallDraw.h"

// ���վ��Ч������ʱ����ǽ����
// ���ֱ�Ӵ�SimpleWallDraw����
class SimpleWindStationDraw : public SimpleWallDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleWindStationDraw) ;

public:
	SimpleWindStationDraw () ;
	virtual ~SimpleWindStationDraw () ;

	virtual void setAllExtraParamsToDefault();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	double m_lableAngle;  // ��ע�Ƕ�
	double m_lableLength; // ��ע����
	double m_hBaseLength;     // ��ע���ֵ�ˮƽ������(��ס���ֵ�ˮƽֱ��)
	double m_textHeight;   // ���ָ߶�

	CString S;
	CString V;
	CString Q;
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleWindStationDraw)
#endif
