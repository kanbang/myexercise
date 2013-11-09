#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cmath>
#include <set>
typedef std::set<int> IntSet;

#define MAX_N 20000

static void Set_Union(const IntSet& aset, IntSet& bset)
{
	IntSet tmp;
	tmp.swap(bset);
	std::set_union(
			aset.begin(), aset.end(),
			tmp.begin(), tmp.end(),
			std::inserter(bset, bset.begin())); 
}

static void Set_Intersect(const IntSet& aset, IntSet& bset)
{
	IntSet tmp;
	tmp.swap(bset);
	std::set_intersection(
			aset.begin(), aset.end(),
			tmp.begin(), tmp.end(),
			std::inserter(bset, bset.begin())); 
}

class SegmentTree
{
	public:
		void build(int nd, int left, int right)
		{
			tree[nd].left = left;
			tree[nd].right = right;
			if (left == right)return;

			int mid = (left + right) >> 1;
			build( 2*nd, left , mid);
			build( 2*nd + 1 , mid + 1 , right);
		}

		void modify(int nd, int left, int right, int num)
		{
			if (nd > MAX_N)return;
			if (left == tree[nd].left && right == tree[nd].right)
			{
				tree[nd].num_set.insert(num);
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

		void search(int nd, int left, int right, IntSet& num_set)
		{
			if (nd > MAX_N)return ;

			if (left == tree[nd].left && right ==tree[nd].right){
				Set_Union(tree[nd].num_set, num_set);
				return;
			}
			if (tree[nd].left == tree[nd].right) return;

			int mid = (tree[nd].left + tree[nd].right) >> 1;
			if (right <= mid){
				search(2*nd, left, right, num_set);
			}
			else if (left >= mid + 1){
				search(2*nd+1, left, right, num_set);
			}
			else {
				IntSet aset, bset;
				search(2*nd, left, mid, aset);
				search(2*nd+1, mid+1, right, bset);
				Set_Intersect(aset, bset);
				Set_Union(bset, num_set);
			}
		}

	private:
		struct node {
			int left, right;
			IntSet num_set;
		};
		node tree[MAX_N];
};

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

static int Discretize(PointArray& X_pos, PointArray& xline)
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

static void FindPointNum(const PointArray& xline, float p, int& x_low, int& x_up)
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

void test2()
{
	SegmentTree st;
	st.build(1, 1, 6);
	st.modify(1, 1, 4, 1);
	st.modify(1, 2, 5, 2);
	st.modify(1, 3, 6, 3);
	IntSet aset;
	std::cout<<"\n[1, 2]------------------------\n";
	st.search(1, 1, 2, aset);
	std::copy(aset.begin(), aset.end(), std::ostream_iterator<int>(std::cout, " "));

	std::cout<<"\n[2, 3]------------------------\n";
	aset.clear();
	st.search(1, 2, 3, aset);
	std::copy(aset.begin(), aset.end(), std::ostream_iterator<int>(std::cout, " "));

	std::cout<<"\n[3, 4]------------------------\n";
	aset.clear();
	st.search(1, 3, 4, aset);
	std::copy(aset.begin(), aset.end(), std::ostream_iterator<int>(std::cout, " "));

	std::cout<<"\n[4, 5]------------------------\n";
	aset.clear();
	st.search(1, 4, 5, aset);
	std::copy(aset.begin(), aset.end(), std::ostream_iterator<int>(std::cout, " "));

	std::cout<<"\n[5, 6]------------------------\n";
	aset.clear();
	st.search(1, 5, 6, aset);
	std::copy(aset.begin(), aset.end(), std::ostream_iterator<int>(std::cout, " "));
}

void test3()
{
	IntSet aset;
	aset.insert(1);
	aset.insert(2);
	aset.insert(6);
	aset.insert(4);
	aset.insert(9);

	IntSet bset;
	bset.insert(3);
	bset.insert(2);
	bset.insert(1);

	std::cout<<"rbegin:"<<*(bset.rbegin())<<std::endl;
	Set_Intersect(aset, bset);
	std::copy(bset.begin(), bset.end(), std::ostream_iterator<int>(std::cout, " "));

}

int main()
{
	//test1();
	test2();

	return 0;
}
