// Fault.h: interface for the CFault class.
//
//////////////////////////////////////////////////////////////////////
#include <afxtempl.h>

#if !defined(AFX_FAULT_H__C21D09EE_7F19_47E0_8C4A_B1AE0BEE6B48__INCLUDED_)
#define AFX_FAULT_H__C21D09EE_7F19_47E0_8C4A_B1AE0BEE6B48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//�ṹ�壬������һ���ϲ㣬һ���ϲ���һϵ�е�������ϲ��������
class CFault: public CObject
{
public:
    CFault();
    virtual ~CFault();

public:
    CArray <double, double> x;
    CArray <double, double> y;
    CArray <double, double> z;
    int iDots; //x,y,z����Ĵ�С,  using!!!
    bool bVisible;	//���ظõ㻹����ʾ�õ㣿
    int no;	//���������ڱ�ʶ�ϲ�����
};

#endif // !defined(AFX_FAULT_H__C21D09EE_7F19_47E0_8C4A_B1AE0BEE6B48__INCLUDED_)
