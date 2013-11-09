#pragma once

#include "VNG_GE.h"

class VNGGE_DLLIMPEXP VNG_Node : public VNG_GE
{
public:
	ACRX_DECLARE_MEMBERS(VNG_Node) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Node () ;
	virtual ~VNG_Node () ;

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

public:
	AcGePoint3d m_pt;     // �ڵ���������
	double m_width;       // ��Բ�Ŀ��
	double m_height;      // ��Բ�ĸ߶�

	double m_textHeight;  // ���ָ߶�
	
	int m_id;       // �ڵ����ƣ�����1��2�ȣ�û��ǰ׺v
} ;

#ifdef VNGGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Node)
#endif
