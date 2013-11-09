//-----------------------------------------------------------------------------
//----- MineGEDraw.h : Declaration of the MineGEDraw
//-----------------------------------------------------------------------------
#pragma once

#include "dbmain.h"
#include "dlimexp.h"
#include "../ArxDbgXdata/XDataStream.h"

// Բ����
#define PI 3.1415926535897932384626433832795

// ��ͼԪ����Ч���������
class MINEGE_DRAW_DLLIMPEXP MineGEDraw : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(MineGEDraw) ;

public:
	virtual ~MineGEDraw () {}

	// �������и��Ӳ���ΪĬ��ֵ
	virtual void setAllExtraParamsToDefault() = 0;

	// ����һ���Ի�����и��Ӳ������޸�
	// ���磬1���޸�˫������Ŀ��
	//		  2���޸�Բ�νڵ�İ뾶��
	virtual void configExtraParams() = 0;

	// �����Ӳ��������仯��ʱ�򣬸��������ĸ��Ӳ���
	// ����˫��������������ȱ仯ʱ����Ҫ���¼�������������
	virtual void updateExtraParams() = 0;

	// ��д�ؼ�����
	virtual void readKeyParam(DrawParamReader& reader) = 0;
	virtual void writeKeyParam(DrawParamWriter& writer) = 0;

	// ��д���Ӳ���
	virtual void readExtraParam(DrawParamReader& reader) = 0;
	virtual void writeExtraParam(DrawParamWriter& writer) = 0;

	// ��MineGEDraw�����ฺ�����names����
	virtual void regPropertyDataNames(AcStringArray& names) const {}

	// ��ͼԪ�ж�ȡ��Ӧ����������
	// ͼԪ��(MineGE��������)����������ݵ�reader��
	virtual void readPropertyDataFromGE(const AcStringArray& values) {}

	// ��ȡ����Ч������С����ΰ�Χ��
	// ���ڱ������ƣ�ʵ��"����"Ч��
	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // Ĭ��ʵ�֣�ʲôҲ����

protected:
	MineGEDraw();;  // �������ܻ��ڹ��캯�������һЩ��Ϣ������Ŀǰ�в�ȷ��(��ʱ�������캯��)
} ;

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGEDraw)
#endif
