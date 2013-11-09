#pragma once

typedef AcArray<COLORREF> AcGeColorArray;

extern bool CreateContourLine(double z, const AcGePoint3dArray& pts, bool bSmooth);
extern bool CreateContourLabel(double z, const AcGePoint3dArray& cnpts, bool bSmooth, const AcGePoint3d& tpt, double textHeight);
extern void CreateContourLabels(double z, const AcGePoint3dArray& cnpts, bool bSmooth, const AcGePoint3dArray& tpts, double textHeight);
extern bool CreateContourFill(const AcGePoint3dArray& pts, int cnpts_num, int bpts_num, COLORREF rgb, bool bSmooth);
extern bool CreateColorimetricPlate(const AcGePoint3d& pt, double width, double height, const AcGeColorArray& all_colors);
extern bool CreateContourInfo(const AcGePoint3dArray& pts, const AcGeDoubleArray& zValues);
