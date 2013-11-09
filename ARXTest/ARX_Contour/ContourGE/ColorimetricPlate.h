#pragma once

#include "ContourGE.h"

struct ARX_Color
{
	int r, g, b;
};
typedef AcArray<ARX_Color> ArxColorArray;

// 比色板
class CONTOURGE_DLLIMPEXP ColorimetricPlate : public ContourGE {

public:
	ACRX_DECLARE_MEMBERS(ColorimetricPlate) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	ColorimetricPlate() ;
	virtual ~ColorimetricPlate () ;

	void setSize(const AcGePoint3d& pt, double width, double height);
	void setLabel(double distance, double textHeight);
	
	void addColor(int r, int g, int b);
	void getAllColors(ArxColorArray& colors);
	
	void addZValue(double z);
	void getAllZValues(AcGeDoubleArray& zValues);

	void clearAll();

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const;
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

private:
	// 颜色数据
	AcArray<ARX_Color> m_colors;
	// z值
	AcGeDoubleArray m_zValues;
	
	// 比色板几何信息
	double m_width;   // 比色板宽度
	double m_height;  // 比色板颜色
	AcGePoint3d m_pt; // 比色板基点

	// 标注文字信息
	double m_distance;    // 标注文字距离比色板的距离
	double m_textHeight;  // 标注文字大小
} ;

#ifdef CONTOURGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ColorimetricPlate)
#endif
