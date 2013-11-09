#pragma once

#include "ContourGE.h"

// 等值线填充
class CONTOURGE_DLLIMPEXP ContourFill : public ContourGE {

public:
	ACRX_DECLARE_MEMBERS(ContourFill) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	ContourFill() ;
	virtual ~ContourFill () ;

	void setPolygon(const AcGePoint3dArray& cnpts, int cnpts_num, int bpts_num, COLORREF rgb);
	void smooth(bool bSmooth);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

private:
	AcGePoint3dArray m_pts;
	int m_cnpts_num;
	int m_bpts_num;
	COLORREF m_rgb;
	
	// 是否使用光滑算法
	bool m_bSmooth;
} ;

#ifdef CONTOURGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ContourFill)
#endif
