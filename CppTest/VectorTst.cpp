#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    typedef vector<int> IntV;
    IntV v;
    v.push_back( 1 );
    v.push_back( 2 );
    //v.push_back(5);
    cout << ( v.end() - v.begin() ) << endl;
    IntV::iterator itr = find( v.begin(), v.end(), 5 );
    cout << ( itr - v.begin() ) << endl;
    cout << v.front() << "\t" << v.back() << endl;
    int a = -1;
    cout << !a << endl;
    int b = 0;
    cout << !b << endl;
    int c = 1;
    cout << !c << endl;

    int d = 5;
    cout << ( d >> 1 ) << endl;

    cout << 1e-6 << endl;
    cout << 1e-3 << "\t" << 0.001 << endl;

    int len = 10;
    cout << ( ( -1 * len ) != -10 ) << endl;

    int* pa;
    cout << ( pa == 0 ) << endl;
    cout << pa << endl;

    double md = 0.0000;
    double precise = 0.0001;

    bool ret = ( md - precise ) <= 0;
    cout << ret << endl;
    return 0;
}
