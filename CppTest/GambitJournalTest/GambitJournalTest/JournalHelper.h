#pragma once

#include "JouDef.h"
#include <fstream>

typedef std::ofstream OutFileStream;

class JournalHelper
{
public:
    /* ���캯�� */
    JournalHelper(void);
    /* �������� */
    ~JournalHelper(void);

    /* ��ӽڵ� */
    void addPoint(const CFD_Point& pt);
    /* ��ӷ�֧ */
    void addEdge(const CFD_Edge& e);

    /* ���һ���� */
    int addFace();
    /* ��ӷ�֧e���� */
    void addEdgeToFace(int e, int face_id);

    /* ����jou�ļ� */
    void write(const char* filename);

private:
    void writeVertices(OutFileStream& outfile);
    void writeEdges(OutFileStream& outfile);
    void writeFaces(OutFileStream& outfile);

    void writeVertex(OutFileStream& outfile, const CFD_Point& pt);
    void writeEdge(OutFileStream& outfile, const CFD_Edge& e);
    void writeFace(OutFileStream& outfile, IntArray& face);

    PointArray m_vertices;   // ���е�����
    EdgeArray m_edges;       // ���з�֧����
    FaceArray m_faces;       // ����������
};
