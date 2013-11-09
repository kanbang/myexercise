#pragma once

#include "MineGE.h"

/*
 * ��ǩ��ͼԪ
 * ����:
 * 	1) ��ǩ����ķ���������ú�����
 * ����ͼԪ����ҪҪ����һ��ͼԪ���������޷��������ڵ�
 * ������������ͼԪ(Host)�����кܶ��"��ǩͼԪ"
 */
class MINEGE_DLLIMPEXP TagGE : public MineGE 
{
public:
	ACRX_DECLARE_MEMBERS(TagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// ��ȡ����ͼԪ
	AcDbObjectId getRelatedGE() const;

	// ���ù�����ͼԪ
	// ֻ�ܺ�MineGEͼԪ����
	void setRelatedGE(const AcDbObjectId& objId);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	// ���m_objId.isNull()==true����ô�Ͳ������ͼ��
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

protected:
	TagGE();

private:
	AcDbObjectId m_objId;    // ��ǩ����������ͼԪ(host)
};

/*
 * �����ǩ��ͼԪ
 * �������������ú���ŵ�
 * ��������������ŵ�(����������Ҫ������Ļ��ֺͲ�νṹ)
 * Ŀǰ��ʱ��ô����
 */
class MINEGE_DLLIMPEXP DirectionTagGE : public TagGE {

public:
	ACRX_DECLARE_MEMBERS(DirectionTagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

	// ��ȡ����Ƕ�
	double getDirectionAngle() const;

	// ���÷���Ƕ�
	void setDirectionAngle(double angle);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	DirectionTagGE();
	DirectionTagGE(const AcGePoint3d& insertPt, double angle);

	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;          // ����Ƕ�
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TagGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DirectionTagGE)
//ACDB_REGISTER_OBJECT_ENTRY_AUTO(TextTagGE)
#endif