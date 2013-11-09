#pragma once

#include "MineGE.h"

class MINEGE_DLLIMPEXP BlockGE : public MineGE {

public:
	ACRX_DECLARE_MEMBERS(BlockGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	// 获取方向角度
	double getDirectionAngle() const;

	// 设置方向角度
	void setDirectionAngle(double angle);

	// 获取块名称
	CString getBlockName() const;

	// 设置块名称
	void setBlockName(const CString& blockName);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	BlockGE();
	BlockGE(const AcGePoint3d& insertPt, double angle, const CString& blockName);

	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 方向角度
	CString m_blockName;     // 块定义名称
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BlockGE)
#endif