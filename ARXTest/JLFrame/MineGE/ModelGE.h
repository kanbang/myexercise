#pragma once

#include "dlimexp.h"

/*
 * ���ݶ���(DataObject)�۲���(�μ����۲���ģʽ)
 * ����:
 *		����ͨ������ͼʱ�������������ͼԪ����ӳ���һ�����߷�֧
 *      �ҷ�ֻ֧ʹ�����ǵ�һ��������(ͨ������)
 */
class MINEGE_DLLIMPEXP ModelGE : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ModelGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// ����ͼԪ��������
	CString getTypeName() const;

	// ��ȡ����Դ
	AcDbObjectId getDataObject() const;

	// ��������Դ
	void setDataObject(const AcDbObjectId& objId);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	ModelGE();

private:
	AcDbObjectId m_objId;  // ����Դ(��MineGE������ͼԪ����DataObject������ݶ���)
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ModelGE)
#endif
