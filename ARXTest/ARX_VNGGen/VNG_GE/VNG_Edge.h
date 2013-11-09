#pragma once

#include "VNG_GE.h"

// 通风网络图--分支
class VNG_GE_DLLIMPEXP VNG_Edge : public VNG_GE 
{
public:
	ACRX_DECLARE_MEMBERS(VNG_Edge) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Edge() ;
	virtual ~VNG_Edge () ;

	void setParam(double m_arrowWidth, double m_arrowLength, double m_edgeTextHeight, double m_needEdge);
	void setName(const CString& name);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
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
	// 分支参数
	AcGePoint3d m_spt, m_ept; // 始末点坐标
	AcGePoint3d m_pt;         // 中间第2点(需要根据控制点计算)

	// 箭头参数
	double m_arrowWidth;      // 箭头宽度
	double m_arrowLength;     // 箭头长度

	// 标签参数
	bool m_needEdge;          // 是否需要显示分支名称
	AcGePoint3d m_tpt;        // 分支名称位置
	double m_textHeight;      // 文字高度

	CString m_name;           // 巷道名称
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Edge)
#endif