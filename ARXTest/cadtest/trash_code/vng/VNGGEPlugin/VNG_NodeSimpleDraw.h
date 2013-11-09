#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class VNG_NodeSimpleDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(VNG_NodeSimpleDraw) ;

public:
	VNG_NodeSimpleDraw () ;
	virtual ~VNG_NodeSimpleDraw () ;

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
	virtual Acad::ErrorStatus subGetOsnapPoints(
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
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	AcGePoint3d m_pt;     // 节点插入点坐标
	double m_width;       // 椭圆的宽度
	double m_height;      // 椭圆的高度

	double m_textHeight;  // 文字高度
	
	int m_id;       // 节点名称，例如1、2等，没有前缀v
} ;

#ifdef VNGGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_NodeSimpleDraw)
#endif
