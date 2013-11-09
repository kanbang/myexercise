#pragma once

#include "DoubleTunnelDraw.h"

// 简单的双线掘进巷道绘制
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
	double m_distance; // 多出的部分(与m_endPt的距离)
	
	AcGePoint3d caclHeadPoint(); // 计算掘进头的点坐标
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleTTunnelDraw)
#endif