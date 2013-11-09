#pragma once

#include "ContourGE.h"

// ��ֵ��
class CONTOURGE_DLLIMPEXP ContourLine : public ContourGE {

public:
	ACRX_DECLARE_MEMBERS(ContourLine) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	ContourLine () ;
	virtual ~ContourLine () ;

	void setZValues(double z);
	void setPoints(const AcGePoint3dArray& cnpts);
	void smooth(bool bSmooth);

	double getZValue() const;
	void getPoints(AcGePoint3dArray& cnpts) const;

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

private:
	double m_zValue;
	AcGePoint3dArray m_pts;
	bool m_bSmooth;      // �Ƿ���й⻬(���Ϊfalse����ʹ�ö���߻���, Ĭ��Ϊtrue)
} ;

#ifdef CONTOURGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ContourLine)
#endif
