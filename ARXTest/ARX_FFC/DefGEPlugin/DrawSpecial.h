#pragma once

#include "JointDrawType.h"

// ����ͼ�λ���
extern void DrawCross(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius);
extern void DrawSin(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double radius);
extern void DrawShaft(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius);
extern void DrawJoint(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, JointDrawType jdt);
