#pragma once

#include "JouDef.h"
#include <fstream>

typedef std::ofstream OutFileStream;

class JournalHelper
{
public:
    /* 构造函数 */
    JournalHelper(void);
    /* 析构函数 */
    ~JournalHelper(void);

    /* 添加节点 */
    void addPoint(const CFD_Point& pt);
    /* 添加分支 */
    void addEdge(const CFD_Edge& e);

    /* 添加一个面 */
    int addFace();
    /* 添加分支e到面 */
    void addEdgeToFace(int e, int face_id);

    /* 生成jou文件 */
    void write(const char* filename);

private:
    void writeVertices(OutFileStream& outfile);
    void writeEdges(OutFileStream& outfile);
    void writeFaces(OutFileStream& outfile);

    void writeVertex(OutFileStream& outfile, const CFD_Point& pt);
    void writeEdge(OutFileStream& outfile, const CFD_Edge& e);
    void writeFace(OutFileStream& outfile, IntArray& face);

    PointArray m_vertices;   // 所有点坐标
    EdgeArray m_edges;       // 所有分支拓扑
    FaceArray m_faces;       // 所有面拓扑
};
