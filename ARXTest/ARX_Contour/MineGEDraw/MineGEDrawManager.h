#pragma once

#include "dlimexp.h"

#include <map>
#include <deque>

class MineGEDraw;
class MineGEDraw_Jig;

struct MineGEDrawInfo
{
	MineGEDraw* pDraw;
	MineGEDraw_Jig* pJig;

	bool operator==(const MineGEDrawInfo& drawInfo)
	{
		return (pDraw == drawInfo.pDraw) && (pJig == drawInfo.pJig);
	}
};

class MINEGE_DRAW_DLLIMPEXP MineGEDrawManager
{
public:
	// ע�����Ч��
	void regGEDraw(const CString& geType, MineGEDraw* pDraw, MineGEDraw_Jig* pJig);

	// ��ע�����Ч��
	void unRegGEDraw(const CString& geType, const CString& realGEDrawName);

	// ָ����ǰ��ͼԪ���͵Ļ���Ч��
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// ��ȡͼԪ���͵ĵ�ǰ����Ч��
	MineGEDraw* getCurrentGEDraw(const CString& geType);

	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	// ��ȡͼԪ���͵����л���Ч��	
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

	// ��ȡͼԪ���͵Ļ���jig
	MineGEDraw_Jig* getGEDraw_Jig(const CString& geType, const CString& realGEDrawName);

private:
	typedef std::deque<MineGEDrawInfo> MineGEDrawInfoList;

	struct DrawListInfo
	{
		MineGEDraw* m_defDraw; // Ĭ�ϻ���Ч��
		MineGEDrawInfoList m_drawInfoList;
	};

	typedef std::map<CString, DrawListInfo> GEDrawMap;
	GEDrawMap m_drawMap;
};