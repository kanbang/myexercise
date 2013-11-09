#pragma once

#include "dlimexp.h"

#include <map>
#include <deque>

class MineGEDraw;

struct MineGEDrawInfo
{
	MineGEDraw* pDraw;

	bool operator==(const MineGEDrawInfo& drawInfo)
	{
		return (pDraw == drawInfo.pDraw);
	}
};

class MINEGE_DRAW_DLLIMPEXP MineGEDrawManager
{
public:
	// 注册可视化效果
	void regGEDraw(const CString& geType, MineGEDraw* pDraw);

	// 反注册可视化效果
	void unRegGEDraw(const CString& geType, const CString& realGEDrawName);

	// 指定当前的图元类型的可视化效果
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// 获取图元类型的当前可视化效果
	MineGEDraw* getCurrentGEDraw(const CString& geType);

	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	// 获取图元类型的所有可视化效果	
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

private:
	typedef std::deque<MineGEDrawInfo> MineGEDrawInfoList;

	struct DrawListInfo
	{
		MineGEDraw* m_defDraw; // 默认可视化效果
		MineGEDrawInfoList m_drawInfoList;
	};

	typedef std::map<CString, DrawListInfo> GEDrawMap;
	GEDrawMap m_drawMap;
};