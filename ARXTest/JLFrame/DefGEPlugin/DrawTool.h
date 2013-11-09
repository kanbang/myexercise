#pragma once

/* ȫ�ֺ���(ʵ����DrawTool.cpp) */

// 3�ضѵ���ʽ���μ�cad������"������������"
//	1) ����: ^ (���¶ѵ�) 
//	2) ����: / (���Ҷѵ����м�һ����б��)
//	3) ����: # (���¶ѵ����м�һ������ )	
extern CString MakeUpperText(const CString& inStr);
extern CString MakeLowerText(const CString& inStr);

extern AcGePoint2d Point3D_To_2D(const AcGePoint3d& pt);
extern AcGePoint3d Point2D_To_3D(const AcGePoint2d& pt);

// ������ο�����
extern void BuildRect(const AcGePoint3d& pt, double angle, double width, double height, AcGePoint3dArray& pts);

// ����ͼ�λ���
extern void DrawLine(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept);
extern void DrawLine(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double length);
extern void DrawCircle(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, bool fill);
extern void DrawArc(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept, bool fill);
extern void DrawArc(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, double startAngle, double sweepAngle, bool fill);
extern void DrawPolyLine(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept, double width);
extern void DrawPolyLine(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double width, double length);
extern void DrawMText(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, const CString& str, double height, AcDbMText::AttachmentPoint ap=AcDbMText::kMiddleCenter);
extern void DrawRect(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double width, double height, bool fill);
extern void DrawSpline(AcGiWorldDraw *mode, const AcGePoint3dArray& pts);
extern void DrawArrow(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double length, double width);
extern void DrawPolygon(AcGiWorldDraw *mode, const AcGePoint3dArray& polygon, bool fill);

extern void CreatePolygonLoop(AcDbHatch* pHatch, const AcGePoint3dArray& pts);
extern void CreateCircleLoop(AcDbHatch* pHatch, const AcGePoint3d& pt, double radius);

extern void CreatePreDefinedHatch(AcDbHatch* pHatch, const CString& patName, double scale);
extern void CreateGradientObject(AcDbHatch* pHatch, const CString& gradName, const AcCmColor& c1, const AcCmColor& c2);

extern void DrawPolygonHatch(AcGiWorldDraw *mode, const AcGePoint3dArray& pts, const CString& patName, double scale);
extern void DrawCircleGradient(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, const CString& gradName, const AcCmColor& c1, const AcCmColor& c2);

// ����ֵ: 
//		0  -- ����
//		1  -- ˳ʱ��
//	   -1  -- ��ʱ��
extern int ClockWise(const AcGePoint3dArray& polygon);

// �պ϶����ƫ��
// ����is_nner--true��ʾ����ƫ�ƣ�false��ʾ����ƫ��
extern bool OffSetPolygon(const AcGePoint3dArray& polygon, double offset, bool is_inner, AcGePoint3dArray& offset_polygon);

