#pragma once

#include "VNG_GE.h"

// ͨ������ͼ--��֧
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
	// ��֧����
	AcGePoint3d m_spt, m_ept; // ʼĩ������
	AcGePoint3d m_pt;         // �м��2��(��Ҫ���ݿ��Ƶ����)

	// ��ͷ����
	double m_arrowWidth;      // ��ͷ���
	double m_arrowLength;     // ��ͷ����

	// ��ǩ����
	bool m_needEdge;          // �Ƿ���Ҫ��ʾ��֧����
	AcGePoint3d m_tpt;        // ��֧����λ��
	double m_textHeight;      // ���ָ߶�

	CString m_name;           // �������
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Edge)
#endif