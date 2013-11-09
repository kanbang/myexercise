#pragma once

#include "../MineGE/LinkedGEDraw.h"

// ���������(Ĭ�Ͼ���1���۵�)
class SingleBlendTunnelDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SingleBlendTunnelDraw);

public:
	SingleBlendTunnelDraw(void);
	~SingleBlendTunnelDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	// �任����(�ƶ���ѡ�񡢾���)--Ŀǰ�ݲ�����"����"����
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	//- Osnap points protocol
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	void drawLine(AcGiWorldDraw* mode, const AcGePoint3d& startPt, const AcGePoint3d& endPt);

	// extra param
	AcGePoint3dArray m_blendPts; // �۵㼯��
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SingleBlendTunnelDraw)
#endif