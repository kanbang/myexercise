#pragma once

#include "ContourGE.h"

// 等值线
class CONTOURGE_DLLIMPEXP ContourLabel : public ContourGE {

public:
	ACRX_DECLARE_MEMBERS(ContourLabel) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	ContourLabel () ;
	virtual ~ContourLabel () ;

	void setZValues(double z);
	void setPoints(const AcGePoint3dArray& cnpts);
	void smooth(bool bSmooth);

	double getZValue() const;
	void getPoints(AcGePoint3dArray& cnpts) const;
	void setTextHeight(double height);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

private:
	double m_zValue;
	AcGePoint3dArray m_pts;
	bool m_bSmooth;      // 是否进行光滑(如果为false，则使用多段线绘制, 默认为true)
	
	AcGePoint3d m_tpt;   // 标注位置
	double m_angle;      // 标注方向

	double m_textHeight; // 标注文字高度
} ;

#ifdef CONTOURGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ContourLabel)
#endif
