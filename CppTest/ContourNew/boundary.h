// boundary.h: interface for the CBoundary class.
// ������ֵ�ߵı߽�
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDARY_H__C53AC22A_1C70_4886_8EAC_9A4FB1823C76__INCLUDED_)
#define AFX_BOUNDARY_H__C53AC22A_1C70_4886_8EAC_9A4FB1823C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afx.h>
#include <afxtempl.h>
#include "structdef.h"



//�߽������
class CBoundary : public CObject
{
public:
    CBoundary();
    virtual ~CBoundary();

private:
    CArray <SPoint, SPoint> m_Points ;
public:
    void BoundByMousePts();//ͨ��������ó�Ա����m_Points��һϵ��ֵ
};

#endif // !defined(AFX_BOUNDARY_H__C53AC22A_1C70_4886_8EAC_9A4FB1823C76__INCLUDED_)
