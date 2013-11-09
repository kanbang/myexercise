#pragma once

/* 
 * Ŀǰ��jigʵ����˸ʮ�����أ�
 * �������Բμ�<<ObjectARX����ʵ���̳�-20090826>>�е�"5.2.2 ��̬���������"�Ĵ���
 */

/* 
 * ��̬ѡ��һ�����������
 * �ö���ο����Ǻ���ģ�Ҳ���ܲ��Ǻ����
 * ֻ��Ҫ�ܹ���ʾһ�����򼴿�
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
