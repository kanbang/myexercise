#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// �����ܱ�
class SingleObturationDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SingleObturationDraw);

public:
	SingleObturationDraw();
	~SingleObturationDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);
	
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
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

private:
	// key param
	AcGePoint3d m_pt;   // ���������
	double m_angle;     // ����Ƕȣ���ʱ�룬��λ���ȣ�

	// extra param
	double m_length;    // ��ͷ���ɳ���(Ĭ��Ϊ44)
};

#ifdef FFC_GEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SingleObturationDraw)
#endif