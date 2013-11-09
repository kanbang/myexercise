#pragma once

#include "ContourGE.h"

struct ARX_Color
{
	int r, g, b;
};
typedef AcArray<ARX_Color> ArxColorArray;

// ��ɫ��
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
	// ��ɫ����
	AcArray<ARX_Color> m_colors;
	// zֵ
	AcGeDoubleArray m_zValues;
	
	// ��ɫ�弸����Ϣ
	double m_width;   // ��ɫ����
	double m_height;  // ��ɫ����ɫ
	AcGePoint3d m_pt; // ��ɫ�����

	// ��ע������Ϣ
	double m_distance;    // ��ע���־����ɫ��ľ���
	double m_textHeight;  // ��ע���ִ�С
} ;

#ifdef CONTOURGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ColorimetricPlate)
#endif
