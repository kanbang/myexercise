#pragma once

#include "MineGEDraw.h"

class MINEGE_DRAW_DLLIMPEXP BlockDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(BlockDraw) ;

public:
	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void readPropertyDataFromGE(const AcStringArray& values);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const;

	virtual Acad::ErrorStatus subGetGripPoints( 
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // Ĭ��ʵ�֣�ʲôҲ����

protected:
	BlockDraw();
	virtual ~BlockDraw();

public:
	// key param
	AcGePoint3d m_insertPt; // ���������
	double m_angle;         // �Ƕ�

	// extra param
	CString m_blockName;    // ������

	// ��ͼԪ��ȡ����������
	// �û�Ҳ�ɸ���ʵ�������޸�ֵ
	AcStringArray m_attValues; // ��������
};

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BlockDraw)
#endif
