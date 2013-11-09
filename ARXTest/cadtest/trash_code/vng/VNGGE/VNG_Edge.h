#pragma once

#include "VNG_GE.h"

// ͨ������ͼ--��֧ͼԪ
class VNGGE_DLLIMPEXP VNG_Edge : public VNG_GE 
{
public:
	ACRX_DECLARE_MEMBERS(VNG_Edge) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	VNG_Edge() ;
	virtual ~VNG_Edge () ;

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

public:
	// ��֧����
	AcGePoint3d m_spt, m_ept; // ʼĩ������
	AcGePoint3d m_pt;         // �м��2��(��Ҫ���ݿ��Ƶ����)

	// ��ͷ����
	double m_arrowWidth;      // ��ͷ���
	double m_arrowLength;     // ��ͷ����

	// ��ǩ����
	bool m_needEdge;          // �Ƿ���Ҫ��ʾ��֧����
	AcGePoint3d m_tpt;        // ��֧����λ��
	double m_textHeight;      // ���ָ߶�

	// id���
	int m_id;
} ;

#ifdef VNGGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Edge)
#endif