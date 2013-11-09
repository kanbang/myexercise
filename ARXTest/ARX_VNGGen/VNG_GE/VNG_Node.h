#pragma once

#include "VNG_GE.h"

// 通风网络图--节点
class VNG_GE_DLLIMPEXP VNG_Node : public VNG_GE
{
public:
	ACRX_DECLARE_MEMBERS(VNG_Node) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Node () ;
	virtual ~VNG_Node () ;

	// 修改参数
	void setParam(double m_nodeWidth, double m_nodeHeight, double m_nodeTextHeight);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	// 获取捕捉点
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	// 获取夹点
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds) const ;

	// 夹点编辑行为
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	AcGePoint3d m_pt;     // 节点插入点坐标
	double m_width;       // 椭圆的宽度
	double m_height;      // 椭圆的高度
	double m_textHeight;  // 文字高度
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Node)
#endif
