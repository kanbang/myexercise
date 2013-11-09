#pragma once

#include "MineGE.h"

/*
 * LinkedGE��ͼԪ����������������桢���ҡ�����������
 * �����ڿ�ϵͳ�������ӹܵ������ã�����ͨ������������ú̿�����ϡ����г���ͨ����
 *
 * ��Ҫ����������1)��������� 2)����Ƕ�
 *
 * ע��MINEGE_DLLIMPEXP���ʾDLL����/����ָ��
 */
class MINEGE_DLLIMPEXP LinkedGE : public MineGE
{
	/* arx�����ɵĴ��� */
	/* ��μ�MineGE���˵�� */
public:
	ACRX_DECLARE_MEMBERS(LinkedGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

	/* �ֶ���ӵĴ��� */
public:
	/* ����ʼĩ������ */
	void setSEPoint(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

	/* ��ȡʼĩ������ */
	void getSEPoint(AcGePoint3d& startPt, AcGePoint3d& endPt) const;

	/* ����(����ʼĩ������) */
	void reverse();

	/* ����������֮��ĽǶ�(X����ʱ����ת) */
	double getAngle() const;

	/* �ֶ���ӵĴ��� */
	/* 
	 * ����LinkedGEͼԪ���ӵ�պ� 
	 * ����ʵ�ֹ��̲μ�:
	 *		1) EdgeJunctionClosure.cpp
	 *		2) "JLFrame�����ĵ�.doc"-->"LinkedGEͼԪ���ӱպϴ���"�½�
	 * ͬMineGE��һ���������ʵ��Ҳ��ί��LinkedGEDraw�����Ŀ��ӻ�������
	 */
public:
	/*
	 * ��ȡʼ�˵�"����"�����췽���(Ĭ��Ϊʼĩ��ֱ�����)
	 * ���ص��������Ǳ�׼������(��������Ϊ1)
	 * ע��"������"��ʾ������պ�
	 */
	AcGeVector3d getStartPointInExtendAngle() const;

	/*
	 * ��ȡĩ�˵�"����"�����췽���(Ĭ��Ϊʼĩ��ֱ����ǵķ�����)
	 * ���ص��������Ǳ�׼������(��������Ϊ1)
	 * ע��"������"��ʾ������պ�
	 */
	AcGeVector3d getEndPointInExtendAngle() const;

	/* ����ʼ����߽��ߣ����ߣ����ཻ(Ĭ�ϲ�����) */
	void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);

	/* ����ĩ����߽��ߣ����ߣ����ཻ(Ĭ�ϲ�����) */
	void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

	/* �ӳ�һ������ */
	void extendByLength(double length);

	/* arx�����ɵĴ��� */
public:
	/* 
	 * ��MFC�����л����ƣ�CAD����dwgOutFields()������ͼԪ�����ݱ��浽DWG�ļ���
	 * ������˵������LinkedGEͼԪ��ʼĩ�����굽DWG�ļ���
	 *
	 * ע1������CAD����(�ƶ�����ת������)�Լ��е�༭ʱ�������һЩ��̬Ч��
	 *       ����Щ��̬Ч�������Ĺ����л�Ƶ���ĵ���dwgOutFields()����
	 * ע2���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * ��MFC�����л����ƣ�CAD����dwgInFields()������DWG�ļ��ж�ȡ���ݵ�ͼԪ��
	 * ������˵����DWG�ļ��ж�ȡʼĩ�����굽LinkedGEͼԪ
	 *
	 * ע1������CAD����(�ƶ�����ת������)�Լ��е�༭ʱ�������һЩ��̬Ч��
	 *       ����Щ��̬Ч�������Ĺ����л�Ƶ���ĵ���dwgInFields()����
	 * ע2���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* ����Ҫ����ʵ�ֵ�MineGE�ി�麯�� */
protected:
	/* 
	 * ��LinkedGE��ͼԪ�Ĺؼ�����(ʼĩ������)д�뵽writer��
	 */
	virtual void writeKeyParam(DrawParamWriter& writer) const;

	/*
	 * ��reader�ж�ȡ2�����ݱ��浽LinkedGEͼԪ�Ĺؼ�����(ʼĩ������)��
	 */
	virtual void readKeyParam(DrawParamReader& reader);

	/* ͨ��arx��������ӵĴ��� */
	/* 
	 * LinkedGEͼԪ��Ҫʵ��һЩ����Ч���������Ҫ����ʵ�ֲ���AcDbEntity���麯�� 
	 * 
	 * ע������sub��ͷ�ķ���������CAD�ڲ�����(AcDbObject->AcDbEntity->MineGE)
	 */
protected:
	/*
	 * ��LinkedGEͼԪ���б任�������Զ��������ӵ�պ�
	 * ���磺
	 *	�����ִ���ƶ�����ƶ����ǰ���ڵ�λ�õ����ӵ�Ҫ�պ�
	 *  �ƶ����������λ�õ����ӵ�պ�ҲҪ����
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	/*
	 * ��LinkedGEͼԪ���ме�༭�������Զ��������ӵ�պ�
	 * ���磺
	 *	ѡ�������ĩ��Ȼ�������ק��ͬ�ϣ���קǰ���λ�ö���Ҫ����պ�
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

	/*
	 * ��CAD������ɾ��LinkedGEͼԪ���Զ��������ӵ�պ�	
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus subErase(Adesk::Boolean erasing);

	/* 
	 * ���캯��
	 * ע������ARX����Ҫ�����е��Զ����඼�����ṩһ��Ĭ�Ϲ��캯��(�޲ι��캯��)
	 *      Ĭ�Ϲ��캯���ķ���Ȩ�޿�����public��Ҳ������protected
	 */
protected:
	/* Ĭ�Ϲ��캯�� */
	LinkedGE();

	/* �в����Ĺ��캯������Ҫ����ʼĩ������ */
	LinkedGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

private:
	void doEdgeGEJunctionClosure();    // ����պ�

protected:
	/* �ؼ�����(��Ҫ������������) */
	AcGePoint3d m_startPt, m_endPt;    // ʼĩ������
} ;

/* 
 * ARX�����ɵĴ��� 
 * ������μ�:MineGE.h�ļ�ĩβ��˵��
 */
#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LinkedGE)
#endif