#pragma once

#include "../MineGE/ModelGE.h"

class VNG_Node : public ModelGE
{
public:
	ACRX_DECLARE_MEMBERS(VNG_Node) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Node () ;
	virtual ~VNG_Node () ;

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
	int m_id;       // �ڵ����ƣ�����1��2�ȣ�û��ǰ׺v
} ;

#ifdef ARX_VNGGEN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Node)
#endif
