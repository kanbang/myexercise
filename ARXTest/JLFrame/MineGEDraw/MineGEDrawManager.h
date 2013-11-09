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
	// ע����ӻ�Ч��
	void regGEDraw(const CString& geType, MineGEDraw* pDraw);

	// ��ע����ӻ�Ч��
	void unRegGEDraw(const CString& geType, const CString& realGEDrawName);

	// ָ����ǰ��ͼԪ���͵Ŀ��ӻ�Ч��
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// ��ȡͼԪ���͵ĵ�ǰ���ӻ�Ч��
	MineGEDraw* getCurrentGEDraw(const CString& geType);

	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	// ��ȡͼԪ���͵����п��ӻ�Ч��	
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

private:
	typedef std::deque<MineGEDrawInfo> MineGEDrawInfoList;

	struct DrawListInfo
	{
		MineGEDraw* m_defDraw; // Ĭ�Ͽ��ӻ�Ч��
		MineGEDrawInfoList m_drawInfoList;
	};

	typedef std::map<CString, DrawListInfo> GEDrawMap;
	GEDrawMap m_drawMap;
};