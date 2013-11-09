#pragma once

#include "ContourGE.h"

// 等值线信息(记录z值和颜色值)
// 无可视化效果
class CONTOURGE_DLLIMPEXP ContourInfo : public ContourGE {

public:
	ACRX_DECLARE_MEMBERS(ContourInfo) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	ContourInfo () ;
	virtual ~ContourInfo () ;

	void setDatas(const AcGePoint3dArray& datas);
	void getDatas(AcGePoint3dArray& datas);

	void setZValues(const AcGeDoubleArray& zValues);
	void getZValues(AcGeDoubleArray& zValues);

	void setColors(const AcArray<COLORREF>& colors);
	void getColors(AcArray<COLORREF>& colors);

	bool fill() const;
	void needFill(bool bFill);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

private:
	AcGeDoubleArray m_zValues;
	AcArray<COLORREF> m_colors;
	AcGePoint3dArray m_datas;
	bool m_bFill;
} ;

#ifdef CONTOURGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ContourInfo)
#endif
