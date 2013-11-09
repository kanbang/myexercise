#pragma once

#include <vector>
using namespace std;

struct GraphInfo
{
	double width;    // ͼ�Ŀ��
	double height;   // ͼ�ĸ߶�
	double ratio;    // ͼ�Ŀ�/�߱���
};

/* 
 * �ڵ�id������ı��
 * ��ʵ�ʽڵ��Ų���ͬ
 * ͨ��lemon graph��id()�����õ�
 */
struct NodeInfo
{
	int id;          // �ڵ�id
	double x, y;     // �ڵ�����
	double cx, cy;   // ��Բ���������
};

/* 
 * ��֧id��ʼĩ�ڵ�sn, tn��������ı��
 * ��ʵ�ʽڵ��Ų���ͬ,
 * ͨ��lemon graph��id()�����õ�
 */
struct EdgeInfo
{
	int id;                  // ��֧id
	int sn, tn;              // ʼĩ�ڵ�
	vector<double> cnx, cny; // ���Ƶ�����
	double tx, ty;           // �ı���ǩ����
};

typedef vector<NodeInfo*> NodeInfoArray;
typedef vector<EdgeInfo*> EdgeInfoArray;
