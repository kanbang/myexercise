#pragma once

#include "GeoDef.h"

// 扩展分支和三角形属性
// 主要用于后续其它算法使用

// 记录分支关联的三角形
// 如果等于-1,则该分支是边界分支
struct EdgeAttrib
{
	DT_Edge e;
	int t[2];

	EdgeAttrib(const DT_Edge& e)
	{
		this->e = e;
		t[0] = -1;
		t[1] = -1;
	}

	// 获取两条边的共点
	int getCoNode(const EdgeAttrib* pEA)
	{
		DT_Edge ee = pEA->e;
		if(ee.s == e.s || ee.t == e.s) return e.s;
		if(ee.t == e.t || ee.s == e.t) return e.t;
		return -1;
	}
	int getTriangle(int i) const
	{
		return t[i];
	}	
	void setTriangle(int id)
	{
		if(id == -1) return;
		if(t[0] == -1)
		{
			t[0] = id;
		}
		else if(t[1] == -1) 
		{
			t[1] = id;
		}
	}
	bool isBoundary() 
	{
		return (t[0] == -1 || t[1] == -1); 
	}
	int findTriangle(int ti) const
	{
		int pos = -1;
		for(int i=0;i<2;i++)
		{
			if(t[i] == ti)
			{
				pos = i; break;
			}
		}
		return pos;
	}
	int getNextTriangle(int ti) const
	{
		int pos = findTriangle(ti);
		if(pos == -1)
			return -1;
		else
			return t[(pos+1)%2];
	}
};

// 记录三角形的3条边
struct TriangleAttrib
{
	DT_Triangle t;
	int e[3];
	bool used;  // 表示追踪三角形过程中是否已被使用

	TriangleAttrib(const DT_Triangle& t)
	{
		this->t = t;
		e[0] = -1;
		e[1] = -1;
		e[2] = -1;
		used = false;
	}

	int getEdge(int i) const
	{
		return e[i];
	}
	DT_Edge getEdgeFromTriangle(int i) const
	{
		DT_Edge e;
		if(i == 0) {e.s = t.p1; e.t = t.p2; }
		if(i == 1) {e.s = t.p2; e.t = t.p3; }
		if(i == 2) {e.s = t.p3; e.t = t.p1; }
		return e;
	}
	void setEdge(int i, int id)
	{
		e[i] = id;
	}
	int findEdge(int ei) const
	{
		int pos = -1;
		for(int i=0;i<3;i++)
		{
			if(e[i] == ei)
			{
				pos = i; break;
			}
		}
		return pos;
	}
	int getNextEdge(int ei) const
	{
		int pos = findEdge(ei);
		if(pos == -1)
			return -1;
		else
			return e[(pos+1)%3];
	}
};

typedef std::vector<EdgeAttrib*> EdgeAttribArray;
typedef std::vector<TriangleAttrib*> TriangleAttribArray;

// 构建扩展属性
extern void BuildAttribs(
	const EdgeArray& ea, 
	const TriangleArray& ta, 
	EdgeAttribArray& eaa, 
	TriangleAttribArray& taa);