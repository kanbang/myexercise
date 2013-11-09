#pragma once

#include "DoubleTunnelDraw.h"

// �򵥵�˫�߾���������
class DoubleTTunnelDraw : public DoubleTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleTTunnelDraw);

public:
	DoubleTTunnelDraw(void);

	virtual void setAllExtraParamsToDefault();

	virtual void configExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);
	virtual void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

public:
	double m_distance; // ����Ĳ���(��m_endPt�ľ���)
	
	AcGePoint3d caclHeadPoint(); // ������ͷ�ĵ�����
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleTTunnelDraw)
#endif