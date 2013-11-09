#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// 单线密闭
class SingleObturationDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SingleObturationDraw);

public:
	SingleObturationDraw();
	~SingleObturationDraw(void);

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

	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

private:
	// key param
	AcGePoint3d m_pt;   // 插入点坐标
	double m_angle;     // 方向角度（逆时针，单位：度）

	// extra param
	double m_length;    // 箭头主干长度(默认为44)
};

#ifdef FFC_GEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SingleObturationDraw)
#endif