#pragma once

#include "dbmain.h"
#include "dlimexp.h"
#include "../ArxDbgXdata/XDataStream.h"

// Բ����
#define PI 3.1415926535897932384626433832795

// ��ͼԪ���ӻ�Ч���������
class MINEGE_DRAW_DLLIMPEXP MineGEDraw : public AcDbEntity 
{
	/* ARX�����ɵĴ��� */
public:
	ACRX_DECLARE_MEMBERS(MineGEDraw) ;

public:
	/* �������� */
	virtual ~MineGEDraw () {}

	/* �������и��Ӳ�����Ĭ��ֵ */
	virtual void setAllExtraParamsToDefault() = 0;

	/*
	 * ����һ���Ի�����и��Ӳ������޸�
	 * ���磬1���޸�˫������Ŀ��
	 */
	virtual void configExtraParams() = 0;

	/*
	 * �����Ӳ��������仯��ʱ�򣬸��������ĸ��Ӳ���
	 * ����˫������������ʼĩ�����ꡢ��ȱ仯ʱ����Ҫ���¼�������������
	 */
	virtual void updateExtraParams() = 0;

	/*
	 * ��д�ؼ�����
	 * ���麯�������������Ҫʵ��!!!
	 */
	virtual void readKeyParam(DrawParamReader& reader) = 0;
	virtual void writeKeyParam(DrawParamWriter& writer) = 0;

	/*
	 * ��д���Ӳ���
	 * ���麯�������������Ҫʵ��!!!
	 */
	virtual void readExtraParam(DrawParamReader& reader) = 0;
	virtual void writeExtraParam(DrawParamWriter& writer) = 0;

	/*
	 * ���������ظ��麯�������names����
	 * ע��names���鱣��Ķ���"�ֶ�"
	 */
	virtual void regPropertyDataNames(AcStringArray& names) const {}

	/*
	 * ͼԪ���ݿ��ӻ�Ч������������ֶ�����(names)
	 * �����ݶ����ж�ȡ��Ӧ�����ݣ�����䵽��������(values)
	 * ע����������Ը��ݿ��ӻ�����Ҫ�����ݽ����޸�
	 * 
	 * ���磺����վͼԪ(WindStation)����2������:
	 *		  names = {"����"��"����"}
	 
	 *	      ������������:
	 *		  vlaues = {"25"��"3.1"}
	 *
	 *        ���Ҫ�������ı䵥λ��ʾ��������޸ģ�
	 *		  Q = 0;  // ����
	 *		  ArxUtilHelper::StringToDouble(values[0], Q);
	 *		  Q = Q*60;      // (m3/s --> m3/min)
	 *		  V = 0;  // ����
	 *	      ArxUtilHelper::StringToDouble(values[1], V);
	 *			
	 *		 ����:Q��V�ǲ��վ���ӻ�Ч���������ж����2����Ա����(double����)
	 *		  
	 */
	virtual void readPropertyDataFromGE(const AcStringArray& values) {}

	/*
	 * ������ӻ�Ч������С�����
	 * ����MineGEͼԪ����"����������"��ʵ��"����"Ч��
	 * 
	 * ע1��BlockDraw���ÿ鶨����Ϊģ�壬�޷�������С�����
	 *      ��ˣ�ʹ��BlockDraw�����ඨ��Ŀ��ӻ�Ч����ͼԪû������Ч��
	 * ע2������Ч��ָ����λ���Ϸ���ͼԪ�Ḳ��λ���·�ͼԪ�Ĳ���ͼ��
	 */
	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // Ĭ��ʵ�֣�ʲôҲ����

protected:
	/* Ĭ�Ϲ��캯�� */
	MineGEDraw();
} ;

/* 
 * ARX�����ɵĴ��� 
 * ���ϰ汾��ARX�У�������Ա��AcDbObject/AcDbEntity�����Զ�����
 * ����Ҫ�ֶ��ĵ���ARX�ṩ�ķ������Զ�����ע�ᵽCAD���νṹ����(Class Hierarchy)
 * �μ���ArxHelper/ArxClassHelper.h
 *
 * ���������Ա���������������������������һЩĪ������Ĵ���
 * ���磺���ͼԪ�����ݿ����CAD��Ļ�Ͽ�����ͼ��
 *
 * �°汾��ARX�ṩ�˸�����(ACDB_REGISTER_OBJECT_ENTRY_AUTO)��
 * �Զ�ע���Զ����ൽCAD���νṹ���У������˿�����Ա����Ļ���
 *
 * MINEGEDRAW_MODULE����MineGEDraw��Ŀ�ж���
 * ��Ŀ���� --> �������� --> C/C++ --> Ԥ������ --> Ԥ����������
 */
#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGEDraw)
#endif
