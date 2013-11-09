#pragma once

/* 
 * 目前的jig实现闪烁十分严重，
 * 后续可以参见<<ObjectARX开发实例教程-20090826>>中的"5.2.2 动态创建多段线"的代码
 */

/* 
 * 动态选择一个多边形区域
 * 该多边形可能是合理的，也可能不是合理的
 * 只需要能够表示一个区域即可
 */
class PolyLineJig : public AcEdJig 
{
public:
	PolyLineJig () ;
	~PolyLineJig () ;

	Adesk::Boolean doJig(AcGePoint3dArray& pts);

protected:
	virtual DragStatus sampler () ;
	virtual Adesk::Boolean update () ;
	virtual AcDbEntity *entity () const ;

private:
	void getAllPoints(AcGePoint3dArray& pts);
	void addPoint(const AcGePoint3d& pt);
	void setPoint(const AcGePoint3d& pt);
	void getLastPoint(AcGePoint3d& pt);
	AcEdJig::DragStatus getNextPoint();
	AcDbPolyline* pPolyLine;
	AcGePoint3d cpt;
	int num;
} ;
