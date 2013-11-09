#pragma once

extern void DrawEllipse(const AcGePoint3d& pt, double width, double height);
extern void DrawText(const AcGePoint3d& pt, const CString& text, double height, AcDb::TextHorzMode hm=AcDb::kTextLeft, AcDb::TextVertMode vm=AcDb::kTextBase);
extern void DrawMText(const AcGePoint3d& pt, double angle, const CString& text, double height);
extern void DrawLines(const AcGePoint3dArray& pts);
extern void DrawPLines(const AcGePoint3dArray& pts, bool isClosed = false);
extern void DrawSplines(const AcGePoint3dArray& pts);
extern bool PostGEToModelSpace(AcDbEntity* pEnt);
extern void DrawArc(const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept);
