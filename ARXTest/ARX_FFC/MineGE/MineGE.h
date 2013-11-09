#pragma once

#include "dlimexp.h"

#include "../MineGEDraw/MineGEDraw.h"
#include "../ArxDbgXdata/XDataStream.h"

// ��ϵͳͼԪ����(������)
class MINEGE_DLLIMPEXP MineGE : public AcDbEntity 
{
public:
	ACRX_DECLARE_MEMBERS(MineGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

	// �����ӿ�
public:
	virtual ~MineGE ();	

	// ��ȡ��������
	CString getTypeName() const;

	// ͨ��jig�ķ�ʽ����ͼԪ
	bool drawByJig();

	// ��ȫ������extra param�Ĳ����󣬸���ͼԪ�Ĳ���
	void configDraw(const CString& drawName);

	// ����ͼԪ���ӻ�Ч��
	void updateDraw();

	// ��ȡ���ݶ���id
	AcDbObjectId getDataObject() const;

	// ����Ҫ����ʵ�ֵ��麯��
protected:
	// ���ؼ�����д�뵽writer��
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const = 0;

	// ��reader�ж�ȡ�ؼ�����
	virtual void pullKeyParamFromReader(DrawParamReader& reader) = 0;

	// AcDbObject���غ���
public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	// AcDbEntity�����غ���
protected:
	// ͼԪ����
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	// �任����(�ƶ���ѡ�񡢾���)--Ŀǰ�ݲ�����"����"����
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	// ��ȡ��׽��
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	// ��ȡ�е�
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds) const ;

	// �е�༭��Ϊ
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	// ��Χ������(����ȷ�����ŵķ�Χ)
	// һ��"����"��3d��Χ��(������)
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

	virtual Acad::ErrorStatus subClose(void);

protected:
	// ���캯��
	MineGE();

	// ���²���(��������)
	// true  -- ������д�뵽draw��
	// false -- ��������ȡ������ge��
	// ��writeParamToGEDraw��readParamFromGEDraw�����ڹ������ظ�(***)
	void updateDrawParam(bool readOrWrite = true) const;

	// ��ȡ��ǰdraw����(��������ʹ�ã�����ֱ�Ӳ���˽������)
	MineGEDraw* getCurrentDraw() const;

private:
	// ���Ʊ�������
	void drawBackground(MineGEDraw* pGEDraw, AcGiWorldDraw *mode);

	// ��д�ؼ�����
	void readKeyParamFromGEDraw(MineGEDraw* pGEDraw);
	void writeKeyParamToGEDraw(MineGEDraw* pGEDraw) const;

	// ��д����ͼԪ���Ӳ���
	void readExtraParamFromGEDraw(MineGEDraw* pGEDraw);
	void writeExtraParamToGEDraw(MineGEDraw* pGEDraw) const;	

	// ��д����(�ؼ������͸��Ӳ���)-- ��������
	void readParamFromGEDraw(MineGEDraw* pGEDraw);
	void writeParamToGEDraw(MineGEDraw* pGEDraw) const;

	// ��ʼ�����еĸ��Ӳ�����xdata��
	void initAllExtraParamsToXData();

	// ��ȡ�Ѵ��ڵ�draw
	void extractExistedDraw(AcStringArray& existedDraw);

	// ��ʼ�����ӻ�Ч��
	void initDraw();

	// ��ʼ����������
	void initPropertyData();

	// д���������ݵ�MineGEDraw��
	void writePropertyDataToGEDraw(MineGEDraw* pGEDraw) const;

	void updateDrawParams(MineGEDraw* pGEDraw);

	void updateCurrentDraw();

private:
	// ��ǰ����Ч��ָ��
	// ͼԪ��ͼ��Ч������ͨ����ָ��ί��ʵ��
	MineGEDraw* m_pCurrentGEDraw;
	// ͼԪ���������ݶ���
	AcDbObjectId m_dataObjectId;
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGE)
#endif