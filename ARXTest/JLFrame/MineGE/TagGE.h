#pragma once

#include "MineGE.h"

/*
 * ��ǩ��ͼԪ
 * ����: �������򡢷��š����
 * ����ͼԪ����ҪҪ����һ������ͼԪ(host)���������޷��������ڵ�
 * ע��������������ͼԪ(Host)�����кܶ��"��ǩͼԪ"
 *
 * ע��MINEGE_DLLIMPEXP���ʾDLL����/����ָ��
 */
class MINEGE_DLLIMPEXP TagGE : public MineGE 
{
	/* arx�����ɵĴ��� */
	/* ��μ�MineGE���˵�� */
public:
	ACRX_DECLARE_MEMBERS(TagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

	/* �ֶ���ӵĴ��� */
public:
	/* ��ȡ����������ͼԪ(host) */
	AcDbObjectId getHost() const;

	/*
	 * ���ù���������ͼԪ(host)
	 * ע��ֻ�ܹ���MineGE������ͼԪ
	 */
	void setHost(const AcDbObjectId& objId);

	/* arx�����ɵĴ��� */
public:
	/* 
	 * ��MFC�����л����ƣ�CAD����dwgOutFields()������ͼԪ�����ݱ��浽DWG�ļ���
	 * ������˵������TagGEͼԪ��"����ͼԪID"(m_objId)��DWG�ļ���
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * ��MFC�����л����ƣ�CAD����dwgInFields()������DWG�ļ��ж�ȡ���ݵ�ͼԪ��
	 * ������˵����DWG�ļ��ж�ȡID��TagGEͼԪ��"����ͼԪ"(m_objId)
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* ͨ��arx��������ӵĴ��� */
	/* 
	 * TagGEͼԪ������������ͼԪ(host)���ڵ�
	 * �������ͼԪ������(m_objId.isNull()==true)��
	 * ��ôTagGEͼԪҲ��Ӧ�ô���ͼ��Ч��(����ͼ��)
	 * 
	 * ע������sub��ͷ�ķ���������CAD�ڲ�����(AcDbObject->AcDbEntity->MineGE)
	 */
protected:
	/* 
	 * �������ͼԪ������(m_objId.isNull()==true)��
	 * ��ôTagGEͼԪ������ͼ��Ч��(����ͼ��)
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

	/* 
	 * ���캯��
	 * ע������ARX����Ҫ�����е��Զ����඼�����ṩһ��Ĭ�Ϲ��캯��(�޲ι��캯��)
	 *      Ĭ�Ϲ��캯���ķ���Ȩ�޿�����public��Ҳ������protected
	 */
protected:
	/* Ĭ�Ϲ��캯�� */
	TagGE();

private:
	/* ��ǩͼԪ����������ͼԪ(host) */
	AcDbObjectId m_objId;
};

/*
 * �����ǩ��ͼԪ������������򡢷��������
 * 
 * ��Ҫ����������1)��������� 2)����Ƕ�
 *
 * ע��MINEGE_DLLIMPEXP���ʾDLL����/����ָ��
 */
class MINEGE_DLLIMPEXP DirectionTagGE : public TagGE 
{
	/* arx�����ɵĴ��� */
	/* ��μ�MineGE���˵�� */
public:
	ACRX_DECLARE_MEMBERS(DirectionTagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

	/* �ֶ���ӵĴ��� */
public:
	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

	// ��ȡ����Ƕ�
	double getDirectionAngle() const;

	// ���÷���Ƕ�
	void setDirectionAngle(double angle);

	/* arx�����ɵĴ��� */
public:
	/* 
	 * ��MFC�����л����ƣ�CAD����dwgOutFields()������ͼԪ�����ݱ��浽DWG�ļ���
	 * ������˵������DirectionTagGEͼԪ��"���������ͷ���Ƕ�"��DWG�ļ���
	 *
	 * ע1���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * ��MFC�����л����ƣ�CAD����dwgInFields()������DWG�ļ��ж�ȡ���ݵ�ͼԪ��
	 * ������˵����DWG�ļ��ж�ȡ"���������ͷ���Ƕ�"��LinkedGEͼԪ
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* 
	 * ���캯��
	 * ע������ARX����Ҫ�����е��Զ����඼�����ṩһ��Ĭ�Ϲ��캯��(�޲ι��캯��)
	 *      Ĭ�Ϲ��캯���ķ���Ȩ�޿�����public��Ҳ������protected
	 */
protected:
	/* Ĭ�Ϲ��캯�� */
	DirectionTagGE();

	/* ���캯����������������ͷ���ǶȲ��� */
	DirectionTagGE(const AcGePoint3d& insertPt, double angle);

	/* ����Ҫ����ʵ�ֵ�MineGE�ി�麯�� */
protected:
	/* 
	 * ��DirectionTagGE��ͼԪ�Ĺؼ�����(���������ͷ���Ƕ�)д�뵽writer��
	 */
	virtual void writeKeyParam(DrawParamWriter& writer) const;

	/*
	 * ��reader�ж�ȡ2�����ݱ��浽DirectionTagGEͼԪ�Ĺؼ�����(���������ͷ���Ƕ�)��
	 */
	virtual void readKeyParam(DrawParamReader& reader);

	/* �ؼ�����(��Ҫ������������) */
private:	
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;          // ����Ƕ�
};

/* 
 * ARX�����ɵĴ��� 
 * ������μ�:MineGE.h�ļ�ĩβ��˵��
 */
#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TagGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DirectionTagGE)
#endif