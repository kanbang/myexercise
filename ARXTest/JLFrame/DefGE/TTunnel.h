#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// ������Ͳ����Ķ�ͷ�����Ϊ���������
// ��Ͳ�;ֲ��ȷ�����ڵ������Ϊ"�������"
class DEFGE_EXPORT_API TTunnel : public LinkedGE
{
public:
	ACRX_DECLARE_MEMBERS(TTunnel) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TTunnel();
	TTunnel(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~TTunnel();

	void setInTunnel(const AcDbObjectId& objId); // ����"�������"
	AcDbObjectId getInTunnel() const;            // ��ȡ"�������"

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

private:
	AcDbObjectId m_inTunnel;  // "�������"
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TTunnel)
#endif
