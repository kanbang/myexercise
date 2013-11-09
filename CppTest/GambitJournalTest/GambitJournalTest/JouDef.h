#pragma once

#include <vector>

struct CFD_Point
{
    double x, y, z;
};

struct CFD_Edge
{
    int s, t;
};

typedef std::vector<CFD_Point> PointArray;
typedef std::vector<CFD_Edge> EdgeArray;
typedef std::vector<int> IntArray;
typedef std::vector<IntArray*> FaceArray;