#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// ͨ������ͼ--��֧ͼԪ
class VNG_EdgeSimpleDraw : public MineGEDraw 
{
public:
	ACRX_DECLARE_MEMBERS(VNG_EdgeSimpleDraw) ;

public:
	VNG_EdgeSimpleDraw() ;
	virtual ~VNG_EdgeSimpleDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();	
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

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

#ifdef VNGGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_EdgeSimpleDraw)
#endif