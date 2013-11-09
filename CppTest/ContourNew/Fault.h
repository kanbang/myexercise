// Fault.h: interface for the CFault class.
//
//////////////////////////////////////////////////////////////////////
#include <afxtempl.h>

#if !defined(AFX_FAULT_H__C21D09EE_7F19_47E0_8C4A_B1AE0BEE6B48__INCLUDED_)
#define AFX_FAULT_H__C21D09EE_7F19_47E0_8C4A_B1AE0BEE6B48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//结构体，表征了一条断层，一条断层由一系列点和这条断层的序号组成
class CFault: public CObject
{
public:
    CFault();
    virtual ~CFault();

public:
    CArray <double, double> x;
    CArray <double, double> y;
    CArray <double, double> z;
    int iDots; //x,y,z数组的大小,  using!!!
    bool bVisible;	//隐藏该点还是显示该点？
    int no;	//保留，用于标识断层的序号
};

#endif // !defined(AFX_FAULT_H__C21D09EE_7F19_47E0_8C4A_B1AE0BEE6B48__INCLUDED_)
