#pragma once

// 通过jig获取多边形
extern bool GetPolygonByJig(AcGePoint3dArray& pts);
// 通过模拟多段线命令获取多边形
extern bool GetPolygonByCmd(AcGePoint3dArray& pts);
