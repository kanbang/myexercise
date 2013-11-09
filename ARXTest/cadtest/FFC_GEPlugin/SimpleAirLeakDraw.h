#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// ��©��Դ��Ч��
class SimpleAirLeakDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SimpleAirLeakDraw);

public:
	SimpleAirLeakDraw();
	~SimpleAirLeakDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	//virtual void regPropertyDataNames(AcStringArray& names) const;
	//virtual void readPropertyDataFromGE(const AcStringArray& values);

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

	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

public:
	// key param
	AcGePoint3d m_pt;   // ���������

	// extra param
	double m_radius;    // Բ�뾶

	// �ⲿ����
	bool m_st;          // Դ����
};

// ©��Դ
class SimpleSourceAirLeakDraw : public SimpleAirLeakDraw
{
public:
	ACRX_DECLARE_MEMBERS(SimpleSourceAirLeakDraw);

public:
	SimpleSourceAirLeakDraw();
};

// ©���
class SimpleSinkAirLeakDraw : public SimpleAirLeakDraw
{
public:
	ACRX_DECLARE_MEMBERS(SimpleSinkAirLeakDraw);

public:
	SimpleSinkAirLeakDraw();
};

#ifdef FFC_GEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleAirLeakDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleSourceAirLeakDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleSinkAirLeakDraw)
#endif