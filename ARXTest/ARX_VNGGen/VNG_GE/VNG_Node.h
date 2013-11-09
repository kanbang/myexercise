#pragma once

#include "VNG_GE.h"

// ͨ������ͼ--�ڵ�
class VNG_GE_DLLIMPEXP VNG_Node : public VNG_GE
{
public:
	ACRX_DECLARE_MEMBERS(VNG_Node) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Node () ;
	virtual ~VNG_Node () ;

	// �޸Ĳ���
	void setParam(double m_nodeWidth, double m_nodeHeight, double m_nodeTextHeight);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	// ��ȡ��׽��
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	// ��ȡ�е�
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds) const ;

	// �е�༭��Ϊ
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	AcGePoint3d m_pt;     // �ڵ���������
	double m_width;       // ��Բ�Ŀ��
	double m_height;      // ��Բ�ĸ߶�
	double m_textHeight;  // ���ָ߶�
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Node)
#endif
