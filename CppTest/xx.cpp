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

    cout << "����a�ĵ�ַ:" << &a << endl;
    cout << "ָ��p1��ŵ�ֵ:" << p1 << endl;

    cout << "����d�ĵ�ַ:" << &d << endl;
    cout << "ָ��p2��ŵ�ֵ:" << p2 << endl;

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
