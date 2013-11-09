#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main()
{
    int a = 20;
    double d = 30.2;

    int* p1 = &a;
    double* p2 = &d;

    cout << "变量a的地址:" << &a << endl;
    cout << "指针p1存放的值:" << p1 << endl;

    cout << "变量d的地址:" << &d << endl;
    cout << "指针p2存放的值:" << p2 << endl;

	vector<bool> aa;
	aa.push_back(1);
	aa.push_back(1);
	aa.push_back(0);
	aa.push_back(1);
	aa.push_back(0);
	aa.push_back(1);
	aa.push_back(1);
	aa.push_back(0);
	aa.push_back(1);
	aa.push_back(1);
	aa.push_back(1);

	copy(aa.begin(), aa.end(), ostream_iterator<bool>(cout," "));
	cout<<endl;
	reverse(aa.begin(), aa.begin()+4);

	copy(aa.begin(), aa.end(), ostream_iterator<bool>(cout," "));
    return 0;
}
