#pragma once

class TriContour : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(TriContour) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	TriContour () ;
	virtual ~TriContour () ;

	void setZValue(double z);
	void setTextHeight(double height);
	void smooth(bool bSmooth);

	void addPoint(const AcGePoint3d& pt);
	void label();

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	Acad::ErrorStatus TriContour::subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const;
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus subExplode(AcDbVoidPtrArray & entitySet) const;

private:
	double m_zValue;
	AcGePoint3dArray m_pts;
	bool m_bSmooth;      // �Ƿ���й⻬(���Ϊfalse����ʹ�ö���߻���,Ĭ��Ϊtrue)
	
	AcGePoint3d m_tpt;   // ��עλ��
	double m_angle;      // ��ע����

	double m_textHeight; // ��ע���ָ߶�
} ;

#ifdef CONTOUR_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TriContour)
#endif
