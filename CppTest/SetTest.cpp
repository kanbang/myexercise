#include <iostream>
#include <set>
using namespace std;

int main()
{
    set<int> s1, s2;
    s1.insert( 1 );
    s1.insert( 3 );
    s1.insert( 5 );
    s1.insert( 7 );

    s2.insert( 1 );
    s2.insert( 3 );
    s2.insert( 5 );
    s2.insert( 8 );

    cout << ( s1 == s2 ) << endl;
    return 0;
}
