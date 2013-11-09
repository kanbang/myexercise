#include <iostream>
using namespace std;

int main()
{
    long x = 0x04000000L;
    cout << sizeof( x ) << endl;
    cout << sizeof( int ) << endl;
    cout << sizeof( unsigned int ) << endl;
    cout << sizeof( unsigned long ) << endl;
    cout << sizeof( double ) << endl;
    return 0;
}
