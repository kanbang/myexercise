#pragma once

#include "MineGE.h"

//���������õ�ͼԪ��������������ҡ������桢���������
// ������ͨ�������п��Կ�����һ����֧
class MINEGE_DLLIMPEXP LinkedGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(LinkedGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	// ����ʼĩ������
	void setSEPoint(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

	// ��ȡʼĩ������
	void getSEPoint(AcGePoint3d& startPt, AcGePoint3d& endPt) const;

	// ����(����ʼĩ��λ��)
	void reverse();

	// ����������֮��ĽǶ�
	double getAngle() const;

public:
	// �������ӵ�պ����
	// ���ص��������Ǳ�׼��������(ģ--��������Ϊ1)
	// ע��"������"��ʾ������պ�
	AcGeVector3d getStartPointInExtendAngle() const;
	AcGeVector3d getEndPointInExtendAngle() const;

	void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);
	void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

	void extendByLength(double length);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );
	virtual Acad::ErrorStatus subErase(Adesk::Boolean erasing);	// ����ɾ��ʱ�ıպ�

protected:
	LinkedGE();
	LinkedGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

private:
	void doEdgeGEJunctionClosure();    // ����պ�

protected:
	AcGePoint3d m_startPt, m_endPt;    // ʼĩ������
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LinkedGE)
#endif