#pragma once

#include "Tunnel.h"

// �������ͼԪ(�����������)
// ��Ϊ�������������ֱ�������һ��ͼ���й���
// Ҳ���Կ��Ǵ�LinkedGE������
class DEFGE_EXPORT_API ArcTunnel : public Tunnel {

public:
	ACRX_DECLARE_MEMBERS(ArcTunnel) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ArcTunnel();
	ArcTunnel(const AcGePoint3d& startPt, const AcGePoint3d& endPt, const AcGePoint3d& thirdPt);
	virtual ~ArcTunnel();

	// ��ȡ/���ð뾶����
	AcGePoint3d getThirdPt() const;
	void setThirdPt(const AcGePoint3d& pt);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// ����3�㻡�ı�ʾ����
	AcGePoint3d m_thirdPt;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ArcTunnel)
#endif
