#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int fun()
{
    cout << "hwllo";
    return 10;
}

class SI
{
public:
    bool operator()( int a, int b )
    {
        return a < b;
    }
};

class TestBase
{
public:
    TestBase( int x )
    {
        a = x;
    }
    ~TestBase()
    {
        dd();
        cout << a << endl;
    }

    virtual void dd()
    {
        a = a - 1;
    }

    int a;
};

#define TT(name) #name
int main()
{
    cout << fun() << endl;

    list<int> aaa;
    aaa.push_back( 1 );
    aaa.push_back( 5 );
    aaa.push_back( 10 );
    aaa.push_back( 123 );
    aaa.push_back( 0 );
    aaa.push_back( 51 );
    aaa.push_back( 6 );
    aaa.push_back( 9 );

//	sort(aaa.begin(), aaa.end(), SI());

    cout << 10e-4 << endl;
    //copy(aaa.begin(), aaa.end(), back_inserter(cout));

    int c[] = {2, 4, 6, 10, 9, 11, 35};
    vector<int> bb( c, c + sizeof( c ) / sizeof( c[0] ) );

    vector<int>::iterator itr = find( bb.begin(), bb.end(), 10 );
    if( itr != bb.end() )
    {
        cout << *( itr + 1 ) << endl;
        cout << *( itr - 1 ) << endl;
    }

    cout << 10e-5 << endl;


    cout << endl << endl;
    TestBase tb( 1000 );

    cout << TT( test ) << endl;

    return 0;
}
