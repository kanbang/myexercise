#pragma once

#include "MineGEDraw.h"

/*
 * ��Щͼ�εĻ��Ʒǳ����ӣ�����ָ����
 * ���������ȫ��ͷ���Ƶķ�����������Ҫ���Ƿǳ���ĸ��Ӳ���
 *
 * ����CADǿ���"�鶨��"(Block)���ܣ�����Ԥ�ȶ��帴��ͼ��
 * Ȼ��BlockDraw���ݿ����ƶ�ȡ�鶨��(���а�����ͼ�κ�����)������ʾͼ��
 * BlockDraw�Ѿ�ʵ���˴󲿷ֹ��ܣ�������Աֻ��Ҫ����������ʵ�ּ����麯��������
 *
 * ���ӻ�������Աʹ��BlockDraw��Ҫ�������²������:
 *		1) ��CAD�ж����
 *		2) ʵ��һ�����ӻ����(ARXģ��)����BlockDraw������
 *		3) �ڹ��캯�����޸ĳ�Ա����m_blockName��ֵ
 *		4) ����鶨���а�������(������)��������ʵ��regPropertyDataNames()������ͬʱ
 *		   �����Ҫ������readPropertyDataFromGE()�������޸Ĵ�ͼԪ��ȡ������������m_attValues
 *		   ע�����뱣֤�鶨������ӵ�"������"�������������ݵ��ֶ���������ͬ��
 *
 * �����ڿ�ļ���˵����
 *	    1) ʹ��block�����Ŀ��Ϊ"�鶨��"(Block Definition)
 *		2) ͨ��insert�������Ŀ��Ϊ"������"(Block Reference)
 *		3) ˫��"������"������"��༭����"������ԣ���Ϊ"���Զ���"(Attribute Definition)
 *		4) ÿ��"���Զ���"����{��ǩ����(tag)���ı�����(text)}
 *		5) ÿ���鶨�嶼��һ��"����¼"(Block Table Record, �����ARX���һ�����ݽṹ������������)
 *		6) ÿ�������ö���һ��ͼ��ʵ�壬������"ģ�Ϳռ����¼"(������Ļ����ʾ��ͼ�ζ�����������)
 *  ����ʾ��
 *	    1) ʹ��ARXDBG�鿴(����ARX�ٷ��Դ�������ARXDBG�õ�)
 */
class MINEGE_DRAW_DLLIMPEXP BlockDraw : public MineGEDraw 
{
public:
	ACRX_DECLARE_MEMBERS(BlockDraw) ;

public:
	/* �������� */
	virtual ~BlockDraw();

	/* �ֶ���ӵĴ��� */
	/* ����˵����μ�MineGEDraw */
public:
	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void readPropertyDataFromGE(const AcStringArray& values);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // ʵ��Ϊ��

	/* ͨ��arx��������ӵĴ��� */
	/* 
	 * BlockDraw�Ѿ�ʵ���˴󲿷ִ�AcDbEntity->MineGEDraw�������sub��ͷ���麯��
	 * 
	 * ע������sub��ͷ�ķ���������CAD�ڲ�����
	 */
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const;

	virtual Acad::ErrorStatus subGetGripPoints( 
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

protected:
	/* Ĭ�Ϲ��캯�� */
	BlockDraw();

public:
	// �ؼ�����
	AcGePoint3d m_insertPt; // ���������
	double m_angle;         // �Ƕ�

	// ���Ӳ���
	CString m_blockName;    // ������

	// ��ͼԪ��ȡ����������
	// �û�Ҳ�ɸ���ʵ�������޸�ֵ
	AcStringArray m_attValues; // ��������
};

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BlockDraw)
#endif
