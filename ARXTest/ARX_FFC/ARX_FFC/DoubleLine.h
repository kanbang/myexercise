#pragma once

// ˫��
class DoubleLine
{
public:
	DoubleLine(const AcGePoint3d& spt, const AcGePoint3d& ept, double width, bool isWorkSurface=false);
	void getSEPoint(AcGePoint3d& spt, AcGePoint3d& ept) const;
	void setStartPoint(const AcGePoint3d& pt);
	void setEndPoint(const AcGePoint3d& pt);

	double getAngle() const;
	double getWidth() const;
	bool isWorkSurface() const;
	void getStartPoints(AcGePoint3d& ls, AcGePoint3d& rs) const;
	void getEndPoints(AcGePoint3d& le, AcGePoint3d& re) const;	

	void getPolygon(AcGePoint3dArray& polygon);
	void appendStartPolygon(const AcGePoint3dArray& polygon);
	void appendEndPolygon(const AcGePoint3dArray& polygon);

	AcGeVector3d getStartPointInExtendAngle() const;       // ��ȡʼ�˵�"����"�����췽���(Ĭ��Ϊֱ�����)
	AcGeVector3d getEndPointInExtendAngle() const;         // ��ȡĩ�˵�"����"�����췽���(Ĭ��Ϊֱ����ǵķ�����)
	void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);
	void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

private:
	void update();
	void caclLeftPoint(double angle, AcGePoint3d& startPt, AcGePoint3d& endPt);
	void caclRightPoint(double angle, AcGePoint3d& startPt, AcGePoint3d& endPt);

private:
	bool m_isWorkSurface;
	AcGePoint3d m_leftStartPt, m_leftEndPt;
	AcGePoint3d m_rightStartPt, m_rightEndPt;
	AcGePoint3d m_startPt, m_endPt;
	double m_width;

	AcGePoint3dArray m_endPolygon;
	AcGePoint3dArray m_startPolygon;
};
