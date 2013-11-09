#pragma once

#include "MineGEDraw.h"

// ��֧��ͼԪ�Ļ���Ч���������
class MINEGE_DRAW_DLLIMPEXP LinkedGEDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(LinkedGEDraw) ;

public:
	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	// �����֧��ͼԪ���ӵ�պϵ��鷽��
public:
	/* 
	* ����ֱ�����Ĭ�ϼ���;
	* �����ڻ�������������������������ͼԪ;
	* �����ڱ߽紦����Ƿ���������б仯��
	* �������������(��������ʾ������պ�)
	*/
	virtual AcGeVector3d getStartPointInExtendAngle() const;       // ��ȡʼ�˵�"����"�����췽���(Ĭ��Ϊֱ�����)
	virtual AcGeVector3d getEndPointInExtendAngle() const;         // ��ȡĩ�˵�"����"�����췽���(Ĭ��Ϊֱ����ǵķ�����)

	virtual void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);  // ����ʼ����߽��ߵ��ཻ(Ĭ�ϲ�����)
	virtual void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine); 	 // ����ĩ����߽��ߵ��ཻ(Ĭ�ϲ�����)

	virtual void reverse();                                                  // ʼĩ�㷴��ʱ����������ҲӦ����仯
	virtual void extendByLength(double length);                              // �����������⣬����length(Ĭ�ϲ�����)

protected:
	LinkedGEDraw();

public:
	AcGePoint3d m_startPt, m_endPt;   // ʼĩ������
} ;

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LinkedGEDraw)
#endif
