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
	AcGePoint3d m_pt;     // 节点插入点坐标
	double m_width;       // 椭圆的宽度
	double m_height;      // 椭圆的高度

	double m_textHeight;  // 文字高度
	
	int m_id;       // 节点名称，例如1、2等，没有前缀v
} ;

#ifdef VNGGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_Node)
#endif
