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
	bool m_bSmooth;      // 是否进行光滑(如果为false，则使用多段线绘制,默认为true)
	
	AcGePoint3d m_tpt;   // 标注位置
	double m_angle;      // 标注方向

	double m_textHeight; // 标注文字高度
} ;

#ifdef CONTOUR_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TriContour)
#endif
