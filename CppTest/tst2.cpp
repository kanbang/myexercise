// freopen("test.in" , "r" , stdin);
/*

   HDOJ 1823 Luck and love 二维线段树 数组实现

*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cmath>

using namespace std;

struct sub_tree{
	int la , ra;
	int max_love;
};
struct segment_tree{
	sub_tree active[2050];
	int lh , rh;
} maintree[260];

#define maxa 253
#define maxb 2045

static void sub_modify(int fa , int nd , int act , int love)
{
	if (nd > maxb)return;
	if (act >= maintree[fa].active[nd].la && act <= maintree[fa].active[nd].ra)
	{
		maintree[fa].active[nd].max_love = max(maintree[fa].active[nd].max_love, love);
	}
	if (maintree[fa].active[nd].la == maintree[fa].active[nd].ra) return;

	int amid = (maintree[fa].active[nd].la+maintree[fa].active[nd].ra)>>1;
	if (act <= amid)
		sub_modify(fa, 2*nd, act, love);
	else
		sub_modify(fa, 2*nd+1, act, love);
}

static void modify(int node , int height , int act , int love)
{
	if (node > maxa)return;
	if (height >= maintree[node].lh && height <= maintree[node].rh){
		sub_modify(node , 1 , act , love);
	}
	if(maintree[node].lh == maintree[node].rh) return;

	int hmid = (maintree[node].lh+ maintree[node].rh) >> 1;
	if (height <= hmid)
		modify(2*node, height, act, love);
	else
		modify(2*node+1, height, act, love);
}

static void sub_build(int fa, int nd, int la, int ra)
{
	maintree[fa].active[nd].la = la;
	maintree[fa].active[nd].ra = ra;
	maintree[fa].active[nd].max_love= -1;
	if (la == ra)return;

	int amid = (la + ra) >> 1;
	sub_build( fa , 2*nd, la , amid);
	sub_build( fa , 2*nd + 1 , amid + 1 , ra);
}

static void build(int node , int lh , int rh , int la , int ra)
{
	maintree[node].lh = lh;
	maintree[node].rh = rh;
	sub_build(node, 1, la, ra);
	if (lh == rh) return;

	int hmid = (lh + rh) >> 1;
	build(2*node, lh, hmid, la, ra);
	build(2*node+1, hmid+1, rh, la, ra);
	return ;
}

static int sub_search(int fa , int nd , int acb , int ace)
{
	if (nd > maxb)return -1;
	if (acb == maintree[fa].active[nd].la && ace ==maintree[fa].active[nd].ra){
		return maintree[fa].active[nd].max_love;
	}
	int amid = (maintree[fa].active[nd].la + maintree[fa].active[nd].ra) >> 1;
	if (ace <= amid){
		return sub_search(fa, 2*nd, acb, ace);
	}
	if (acb >= amid + 1){
		return sub_search(fa, 2*nd+1, acb, ace);
	}
	//if (acb <= amid && ace > amid){
	return max(sub_search(fa, 2*nd, acb, amid), sub_search(fa, 2*nd+1, amid+1, ace));
	//}
}

static int search(int node , int hb , int he , int acb , int ace)
{
	if (node > maxa)return -1;

	if (hb == maintree[node].lh && he == maintree[node].rh){
		return sub_search(node , 1 , acb , ace);
	}
	int hmid = ( maintree[node].lh + maintree[node].rh ) >> 1;
	if (he <= hmid){
		return search(2*node, hb, he, acb, ace);
	}
	if (hb >= hmid + 1){
		return search(2*node+1, hb, he, acb, ace);
	}
	//if (hb <= hmid && he > hmid){
	return max(search(2*node, hb, hmid, acb, ace) , search(2*node+1 , hmid + 1 , he , acb , ace));
	//}
	//return 0;
}

struct Point 
{
	Point(float _pt) : pt(_pt) {}
	float pt;
	int num;
	bool operator<(const Point& a) const
	{
		return pt < a.pt;
	}
	bool operator==(const Point& a) const
	{
		return abs(pt-a.pt) < 0.0001;
	}
};

std::ostream& operator<<(std::ostream& os, const Point& a)
{
	os << a.pt <<" --> "<<a.num;
	return os;
}

typedef std::vector<Point> PointArray;

static int discretize(PointArray& X_pos, PointArray& xline)
{
	std::copy(X_pos.begin(), X_pos.end(), std::back_inserter(xline));

	for(int i=0;i<X_pos.size()/2;i++)
	{
		xline[2*i].num = -(i+1);
		xline[2*i+1].num = i+1;
	}

	std::sort(xline.begin(), xline.end());
	PointArray::iterator itr = std::unique(xline.begin(), xline.end());
	xline.erase(itr, xline.end());

	Point tmp = xline.front();
	int tp = 1;
	for(int i=0;i<xline.size();i++)
	{
		if(!(tmp == xline[i]))
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
		xline[i].num = tp;
	}
	return tp;
}

static void findNum(const PointArray& xline, float p, int& x_low, int& x_up)
{
	std::pair<PointArray::const_iterator,PointArray::const_iterator> b1;
	b1=std::equal_range (xline.begin(), xline.end(), Point(p));

	int low=-1, up= -1;
	bool c1 = (b1.first == b1.second && b1.first != xline.begin() && b1.first != xline.end());
	bool c2 = (b1.first != b1.second);
	if(c1 || c2)
	{
		low= b1.first-xline.begin();
		up= b1.second-xline.begin();
	}
	if(c1)
	{
		low--;
	}

	x_low=-1; x_up=-1;
	if(low!= -1 && up!= -1)
	{
		x_low= xline[low].num;
		x_up= xline[up].num;
	}
}

int main()
{
	PointArray X_pos, Y_pos;
	X_pos.push_back(Point(100));
	X_pos.push_back(Point(180));
	X_pos.push_back(Point(160));
	X_pos.push_back(Point(300));
	X_pos.push_back(Point(130));
	X_pos.push_back(Point(220));
	//X_pos.push_back(Point(140));
	//X_pos.push_back(Point(250));
	//X_pos.push_back(Point(180));
	//X_pos.push_back(Point(220));

	Y_pos.push_back(Point(50));
	Y_pos.push_back(Point(80));
	Y_pos.push_back(Point(40));
	Y_pos.push_back(Point(90));
	Y_pos.push_back(Point(70));
	Y_pos.push_back(Point(120));
	//Y_pos.push_back(Point(60));
	//Y_pos.push_back(Point(170));
	//Y_pos.push_back(Point(60));
	//Y_pos.push_back(Point(90));

	PointArray xline, yline;
	int x_tp = discretize(X_pos,xline);
	int y_tp = discretize(Y_pos,yline);

	std::copy(X_pos.begin(), X_pos.end(), std::ostream_iterator<Point>(cout, "\n"));
	cout<<"----------------------------"<<endl;
	std::copy(Y_pos.begin(), Y_pos.end(), std::ostream_iterator<Point>(cout, "\n"));

	cout<<"-----------after sort-------------"<<endl;
	std::copy(xline.begin(), xline.end(), std::ostream_iterator<Point>(cout, "\n"));
	cout<<"----------------------------"<<endl;
	std::copy(yline.begin(), yline.end(), std::ostream_iterator<Point>(cout, "\n"));
	cout<<"x_tp:"<<x_tp<<endl;
	cout<<"y_tp:"<<y_tp<<endl;

	build(1, 1, x_tp, 1, y_tp);
	for(int i=0;i<X_pos.size()/2;i++)
	{
		modify(1, X_pos[2*i].num, Y_pos[2*i].num, i);
		modify(1, X_pos[2*i+1].num, Y_pos[2*i+1].num, i);
	}

	int x_low=-1, x_up=-1, y_low = -1, y_up = -1;
	findNum(xline, 120, x_low, x_up);
	findNum(yline, 58, y_low, y_up);
	std::cout << "x=120: --> min=" << x_low << " max=" << x_up << '\n';
	std::cout << "y=58: --> min=" << y_low << " max=" << y_up << '\n';

	int ret = search(1, x_low, x_up, y_low, y_up);
	cout<<ret<<endl;

	findNum(xline, 190, x_low, x_up);
	findNum(yline, 90, y_low, y_up);
	std::cout << "x=190: --> min=" << x_low << " max=" << x_up << '\n';
	std::cout << "y=90: --> min=" << y_low << " max=" << y_up << '\n';

	ret = search(1, x_low, x_up, y_low, y_up);
	cout<<ret<<endl;

	return 0;
}
