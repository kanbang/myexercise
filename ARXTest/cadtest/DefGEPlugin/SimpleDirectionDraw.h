#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class SimpleDirectionDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SimpleDirectionDraw);

public:
	SimpleDirectionDraw();
	~SimpleDirectionDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);
	
	virtual void regPropertyDataNames(AcStringArray& names) const;
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
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

private:
	AcGePoint3d caclArrowTailPt() const; // �����ͷĩ�˵ĵ�����
	void drawPollutionFlag(AcGiWorldDraw *mode); // �����۷���(2����Բ)

private:
	// key param
	AcGePoint3d m_insertPt;   // ���������
	double m_angle;           // ����Ƕȣ���ʱ�룬��λ���ȣ�

	// extra param
	double m_length;          // ��ͷ���ɳ���(Ĭ��Ϊ180)
	double m_arrow_theta;     // ��ͷ�н�(Ĭ��Ϊ20��)
	double m_arrow_length;    // ��ͷ��֧����(Ĭ��Ϊ50)

	// ����������Բ
	double m_radius;          // �۷�����������(2����Բ)�İ뾶

	/* 
	 * ����
	 * enum VENT_TYPE
	 * {
	 * 	 VT_IN       = 0,   // ����
	 *	 VT_OUT      = 1,   // �ط�
	 * };
	 */
	int m_vt;                 // ͨ������
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleDirectionDraw)
#endif