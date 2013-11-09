#include <iostream>
#include <map>
#include <string>

using namespace std;

class Test
{
public:
    Test( const string& name ) : m_name( name ) {}
    string m_name;
};

int main()
{
    map<string, double> dmap;
    dmap["a"] = 1.6;
    dmap["b"] = 2.1;
    dmap["c"] = 3.6;
    dmap["d"] = 4.1;
    dmap["e"] = 5.6;
    dmap["f"] = 6.1;

    map<string, double>::iterator itr = dmap.find( "b" );

    itr->second = 100.1;

    cout << dmap["b"] << endl;

    map<string, Test*> test_map;
    test_map["first"] = new Test( "first" );
    test_map["second"] = new Test( "second" );

    map<string, Test*>::iterator itr2 = test_map.find( "second" );
    delete itr2->second;
    itr2->second = new Test( "second_plus" );

    cout << test_map["second"]->m_name << endl;

    for( map<string, Test*>::iterator itr = test_map.begin(); itr != test_map.end(); itr++ )
    {
        delete itr->second;
    }
    return 0;
}
