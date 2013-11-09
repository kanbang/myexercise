#include<iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>

const int maxn=4000;

struct sub_tree
{
	int left,right;
	int mid;
	float max;
};

struct sgement_tree
{
	int left,right;
	int mid;
	sub_tree Y[maxn];
};

sgement_tree X[400];

void sub_build(sgement_tree &x,int id,int from,int to)
{
	x.Y[id].left=from;
	x.Y[id].right=to;
	x.Y[id].mid=(from+to)>>1;
	x.Y[id].max=-1;

	if(from==to) return ;

	sub_build(x,2*id,from,x.Y[id].mid);
	sub_build(x,2*id+1,x.Y[id].mid+1,to);
}

void build(int id,int from,int to)
{
	X[id].left=from;
	X[id].right=to;
	X[id].mid=(from+to)>>1;
	sub_build(X[id],1,1,1001);

	if(from==to)return ;

	build(2*id,from,X[id].mid);
	build(2*id+1,X[id].mid+1,to);
}

int sub_modify(sgement_tree &x,int id,int a,float luck)
{
	if(luck>x.Y[id].max)x.Y[id].max=luck;

	if(x.Y[id].left==x.Y[id].right)
		return 1;
	if(a<=x.Y[id].mid)
		sub_modify(x,2*id,a,luck);
	else 
		sub_modify(x,2*id+1,a,luck);
	return 1;
}

int modify(int id,int h,int a,float luck)
{    

	sub_modify(X[id],1,a,luck);

	if(X[id].left==X[id].right)
		return 1;
	if(h<=X[id].mid)
		modify(2*id,h,a,luck);
	else 
		modify(2*id+1,h,a,luck);
	return 1;
}

float sub_search(sgement_tree &x,int id,int a1,int a2)
{
	if(x.Y[id].left==a1 && x.Y[id].right==a2)
		return x.Y[id].max;

	if(a2<=x.Y[id].mid)
		return sub_search(x,2*id,a1,a2);
	if(a1>x.Y[id].mid)
		return sub_search(x,2*id+1,a1,a2);

	float l1=sub_search(x,2*id,a1,x.Y[id].mid);
	float l2=sub_search(x,2*id+1,x.Y[id].mid+1,a2);

	return l1>l2? l1:l2;
}

float search(int id,int h1,int h2,int a1,int a2)
{   
	float l1,l2;

	if(X[id].left==h1&&X[id].right==h2)
	{
		return sub_search(X[id],1,a1,a2);
	}
	if(h2<=X[id].mid)
		return search(2*id,h1,h2,a1,a2);
	if(h1>X[id].mid)
		return search(2*id+1,h1,h2,a1,a2);

	l1=search(2*id,h1,X[id].mid,a1,a2);
	l2=search(2*id+1,X[id].mid+1,h2,a1,a2);

	return l1>l2? l1:l2;
}

struct Point 
{
	Point(float _pt) : pt(_pt) {}
	float pt;
	int num;
	bool operator<(const Point& a)
	{
		return pt < a.pt;
	}
};

typedef std::vector<Point> PointArray;

static bool CmpFloat(float a, float b)
{
	return abs(a-b) < 0.0001;
}

static int Discretize(PointArray& X_pos)
{
	PointArray xline;
	std::copy(X_pos.begin(), X_pos.end(), std::back_inserter(xline));

	for(int i=0;i<X_pos.size()/2;i++)
	{
		xline[2*i].num = -(i+1);
		xline[2*i+1].num = i+1;
	}

	std::sort(xline.begin(), xline.end());

	float  tmp = xline.front().pt;
	int tp = 1;
	for(int i=0;i<xline.size();i++)
	{
		if(!CmpFloat(xline[i].pt, tmp))
		{
			tp++;
			tmp = xline[i].pt;
		}
		int k = 0;
		if(xline[i].num < 0)
		{
			k = 2*(-xline[i].num-1);
		}
		else
		{
			k = 2*(xline[i].num-1)+1;
		}
		X_pos[k].num = tp;
	}
	return tp;
}

int main()
{
	int H,A,H1,H2,A1,A2,m;
	float L,L1,L2,luck;
	char c;

	PointArray X_pos, Y_pos;
	X_pos.push_back(Point(100));
	X_pos.push_back(Point(180));
	X_pos.push_back(Point(160));
	X_pos.push_back(Point(300));
	X_pos.push_back(Point(130));
	X_pos.push_back(Point(220));
	X_pos.push_back(Point(180));
	X_pos.push_back(Point(220));

	Y_pos.push_back(Point(50));
	Y_pos.push_back(Point(80));
	Y_pos.push_back(Point(40));
	Y_pos.push_back(Point(90));
	Y_pos.push_back(Point(70));
	Y_pos.push_back(Point(120));
	Y_pos.push_back(Point(60));
	Y_pos.push_back(Point(90));

	int x_tp = Discretize(X_pos);
	int y_tp = Discretize(Y_pos);

	for(int i=0;i<X_pos.size();i++)
	{
		std::cout<<X_pos[i].pt<<" --> " <<X_pos[i].num<<std::endl;
	}

	build(1,1,x_tp);
	/*
	H = 160-99; A = int(50.5*10)+1; luck = 60.0f;
	modify(1,H,A,luck);
	H = 165-99; A = int(30.0*10)+1; luck = 80.5f;
	modify(1,H,A,luck);
	H = 166-99; A = int(10.0*10)+1; luck = 50.0f;
	modify(1,H,A,luck);
	H = 170-99; A = int(80.5*10)+1; luck = 77.5f;
	modify(1,H,A,luck);

	H1 = 150; H2 = 166; L1 = 10.0f; L2 = 60.0f;
	A1=int(L1*10)+1; A2=int(L2*10)+1;
	luck=search(1,H1-99,H2-99,A1,A2);
	if(luck<0)printf("-1\n");
	else printf("%.1lf\n",luck);

	H1 = 166; H2 = 177; L1 = 10.0f; L2 = 50.0f;
	A1=int(L1*10)+1; A2=int(L2*10)+1;
	luck=search(1,H1-99,H2-99,A1,A2);
	if(luck<0)printf("-1\n");
	else printf("%.1lf\n",luck);

	H = 166-99; A = int(40.0*10)+1; luck = 99.9f;
	modify(1,H,A,luck);

	H1 = 166; H2 = 177; L1 = 10.0f; L2 = 50.0f;
	A1=int(L1*10)+1; A2=int(L2*10)+1;
	luck=search(1,H1-99,H2-99,A1,A2);
	if(luck<0)printf("-1\n");
	else printf("%.1lf\n",luck);
	*/

	return 0;
}
