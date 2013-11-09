#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <afxtempl.h>
#include <afxcoll.h>
#include "fault.h"
#include "structdef.h"
#include "somefuncs.h"

class CTriangle : public CObject
{
private:
    //CArray<double, double> x,y,z;
    CArray<double, double> x, y, z;

//	double x[2000], y[2000], z[2000];	//��0��ʼ��ɢ������
    int kp;//ɢ��ĸ���,Ҳ��x[],y[]�з����ĸ���.

//	int ibcount; //׷�ٵ������εĸ���
public:
    double xmin,xmax,ymin,ymax,epsilon;
    //char *pwd;	//Dim pwd As String


private://׷���γɵ�����������
    CTypedPtrArray<CObArray, CTriSet*> m_trisets;	//�������е������μ��� (CTriSet *)
    CTypedPtrArray<CObArray, CFault*>  m_faults;	//�������еĶϲ�����, ΪCFault�ṹ

private:
    CRgn faultsRgn;		//��drawtriangle������
    CArray<SPoint, SPoint> m_Boundary;	//��ֵ��ͼ�ı߽�


//����
public:
    CTriangle();
    virtual ~CTriangle();

private:	//���ϲ�����
    void Set_faults(CString filename);
    void DrawFaults(bool bSmooth, CDC* dc);

private:
    void read_data(CString Filename);
    void tie2(double x1, double y1, double x2, double y2, double x3, double y3,
              double x4, double y4, double& xc, double& yc, int& status1);
    bool GetNextTriangle(SDot& dot1_index, SDot& dot2_index, int& InTri, const double zvalue, double& zbak, const int* iUsedTri, const int iUsedTriCount, CTriSet* triset);
    void SaveUsedTris(int* iUsedTri, int& iUsedTriCount, const int iTriNum);
    bool SearchTriInUsedTris(const int srcTri, const int* iUsedTri, const int iUsedTriCount);
    void Draw(const int k, const int L, CDC* dc);
    void DrawTris(CDC* dc, int n=-1);

private:
    SPoint SDot2SPoint(SDot srcDot);	//SDOT��SPoint�ṹת��
    double InsertZinDot(SDot destDot);	//�ڶϲ��ϲ���Zֵ

    void CreateTriSet(CDC* dc, SDot theDot, CArray<SDot, SDot>& usedDots); //׷�ٲ��γ����е������μ���
    void CreateTriSetEx(CDC* dc);
    bool Function01_Ex(const SDot dot1, const SDot dot2);
    bool Function02_Ex(const SDot ib1L, const SDot ib2L, const SDot ib3L);
    bool SearchExtTri(SDot dot1, SDot dot2, SDot dot3, SDot& searchedDot, CArray<SDot, SDot>& usedDots);
    bool IsValidNewTri(int& kc, double& alpha, double& dd, const SDot srcdot1, const SDot srcdot2, const SDot srcdot3, const SDot desdot, const SDot lastdot, CArray<SDot, SDot>& usedDots);
    void SDot2XY(const SDot srcdot, double& x0, double& y0);
    void SDot2XY(const SDot srcdot, CPoint& point);
    bool IsLineCrossFaults(const SDot ib1, const SDot ib2, UINT& iLinkFault); //�ж϶������Ƿ��ڶϲ��ͬһ��
    bool IsLineCrossFaults(const double x1, const double y1, const double x2, const double y2, UINT& iLinkFault); //�ж϶������Ƿ��ڶϲ��ͬһ��
    int DotRelationshipToTriangle(SDot dot1, SDot dot2, SDot dot3, SDot destdot);
    int DotRelationshipToTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x0, double y0);

    void CreateContour(CDC* dc, const double zvalue, CTriSet* triset);

public:
    void DrawBoundary(CDC* dc);
    //�����������
    void DrawTriangle(CDC* dc);
    //����ֵ��
    void CreateContourEx(CDC* dc, const double zvalue);
    bool IsInControlDots(const POINT point, double& zval);

public:
    CDC* mydc;
public:
    void InitialTriangle();
    void Serialize(CArchive& ar);
};


