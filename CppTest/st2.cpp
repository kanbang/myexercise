// freopen("test.in" , "r" , stdin);
/*

   HDOJ 1823 Luck and num 二维线段树 数组实现

*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cmath>

using namespace std;

struct tree{
	int left, right;
	int num;
} tree[260];

#define MAX_N 2045

static void build(int nd, int left, int right)
{
	tree[nd].left = left;
	tree[nd].right = right;
	tree[nd].num= -1;
	if (left == right)return;

	int mid = (left + right) >> 1;
	build( 2*nd, left , mid);
	build( 2*nd + 1 , mid + 1 , right);
}

static void modify(int nd, int left, int right, int num)
{
	if (nd > MAX_N)return;
	if (left == tree[nd].left && right == tree[nd].right)
	{
		tree[nd].num = max(tree[nd].num, num);
		cout<<" left:"<<left<<" right:"<<right
			<<" node_left:"<<tree[nd].left
			<<" node_right:"<<tree[nd].right
			<<" num:"<<tree[nd].num<<endl;
	}
	if (tree[nd].left == tree[nd].right) return;

	int mid = (tree[nd].left+tree[nd].right)>>1;
	if (right <= mid){
		modify(2*nd, left, right, num);
	}
	else if (left >= mid + 1){
		modify(2*nd+1, left, right, num);
	}
	else {
		modify(2*nd, left, mid, num);
		modify(2*nd+1, mid+1, right, num);
	}
}

static int search(int nd, int left, int right)
{
	if (nd > MAX_N)return -1;

	if (left == tree[nd].left && right ==tree[nd].right){
		return tree[nd].num;
	}
	int mid = (tree[nd].left + tree[nd].right) >> 1;
	if (right <= mid){
		return search(2*nd, left, right);
	}
	if (left >= mid + 1){
		return search(2*nd+1, left, right);
	}
	//if (left <= mid && right > mid){
	return max(search(2*nd, left, mid), search(2*nd+1, mid+1, right));
	//}
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

typedef std::vector<Point> PointArrighty;

static int discretize(PointArrighty& X_pos, PointArrighty& xline)
{
	std::copy(X_pos.begin(), X_pos.end(), std::back_inserter(xline));

	for(int i=0;i<X_pos.size()/2;i++)
	{
		xline[2*i].num = -(i+1);
		xline[2*i+1].num = i+1;
	}

	std::sort(xline.begin(), xline.end());
	PointArrighty::iterator itr = std::unique(xline.begin(), xline.end());
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

static void findNum(const PointArrighty& xline, float p, int& x_low, int& x_up)
{
	std::pair<PointArrighty::const_iterator,PointArrighty::const_iterator> b1;
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

void test1()
{
	PointArrighty X_pos, Y_pos;
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

	PointArrighty xline, yline;
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

	build(1, 1, x_tp);
	for(int i=0;i<X_pos.size()/2;i++)
	{
		modify(1, X_pos[2*i].num, X_pos[2*i+1].num, i);
	}

	int x_low=-1, x_up=-1;
	findNum(xline, 170, x_low, x_up);
	std::cout << "x=170: --> min=" << x_low << " max=" << x_up << '\n';
	int ret = search(1, x_low, x_up);
	cout<<ret<<endl;

	findNum(xline, 190, x_low, x_up);
	std::cout << "x=190: --> min=" << x_low << " max=" << x_up << '\n';
	ret = search(1, x_low, x_up);
	cout<<ret<<endl;

	findNum(xline, 570, x_low, x_up);
	std::cout << "x=170: --> min=" << x_low << " max=" << x_up << '\n';
	ret = search(1, x_low, x_up);
	cout<<ret<<endl;
}

void test2()
{
	build(1, 1, 4);
		modify(1, 1, 3, 1);
		modify(1, 2, 4, 2);
	ret = search(1, x_low, x_up);
	cout<<ret<<endl;

}

int main()
{
	//test1();
	test2();
	return 0;
}
