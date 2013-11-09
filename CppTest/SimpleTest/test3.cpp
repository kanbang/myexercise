#include <iostream>
#include <vector>
#include <cfloat>
using namespace std;

#define DAO_HELPER_SYMBOL_STRINGIZE(symbol) DO_SYMBOL_STRINGIZE(symbol)
#define DO_SYMBOL_STRINGIZE(symbol) #symbol

int main()
{
    vector<int> ii;
    cout << ( ii.begin() == ii.end() );

    unsigned long sev = 1;
    unsigned long fac = 10;
    unsigned long code = 3205;
    cout << ( ( ( unsigned long )( sev ) << 31 ) | ( ( unsigned long )( fac ) << 16 ) | ( ( unsigned long )( code ) ) ) << endl;

    cout << _isnan( 100.0 ) << endl;
    double a = DBL_MAX;
    cout << _isnan( DBL_MAX ) << endl;
    cout << ( a == DBL_MAX ) << endl;
    cout << _finite( DBL_MAX ) << endl;

    cout << DAO_HELPER_SYMBOL_STRINGIZE( 字段 ) << endl;
    return 0;
}
