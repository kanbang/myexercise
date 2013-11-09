#pragma once

#include "../MineGE/ModelGE.h"

// ͨ������ͼ--��֧ͼԪ
class VNG_Edge : public ModelGE 
{
public:
	ACRX_DECLARE_MEMBERS(VNG_Edge) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Edge() ;
	virtual ~VNG_Edge () ;

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

	// id���
	int m_id;
} ;

#ifdef ARX_VNGGEN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Edge)
#endif