#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// 简单的三角形掘进巷道绘制效果
class SimpleTTunnelDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SimpleTTunnelDraw) ;

public:
	SimpleTTunnelDraw(void);
	~SimpleTTunnelDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	void drawLine( AcGiWorldDraw *mode, AcGePoint3d& startPt, AcGePoint3d& endPt );
	void drawText(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle, double height);

public:
	// key param
	AcGePoint3d m_insertPt; // 插入点坐标
	double m_angle; // 角度

	// extra param
	double m_gap; // 距离掘进头末端的小间距
	double m_width;
	double m_distance; // 距离
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleTTunnelDraw)
#endif
