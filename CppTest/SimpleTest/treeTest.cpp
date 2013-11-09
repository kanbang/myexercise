#include <iostream>
#include <map>
#include <string>
using namespace std;

struct UnitInfo
{
    string parent_unit;
    int unit_num;
};

typedef map<string, UnitInfo> UnitInfoMap;

void initUninInfoMap( UnitInfoMap& uim )
{
    UnitInfo info;
    info.parent_unit = ""; // ROOT
    info.unit_num = 0;
    uim.insert( UnitInfoMap::value_type( "中国", info ) );

    info.parent_unit = "中国";
    uim.insert( UnitInfoMap::value_type( "武汉", info ) );

    info.parent_unit = "中国";
    uim.insert( UnitInfoMap::value_type( "北京", info ) );

    info.parent_unit = "武汉";
    uim.insert( UnitInfoMap::value_type( "京山", info ) );
}

void caclUnitInfoMap( UnitInfoMap& uim )
{
    while( true )
    {
        bool hasMoreToRevise = false;
        for( UnitInfoMap::iterator itr = uim.begin(); itr != uim.end(); itr++ )
        {
            if( itr->second.parent_unit.empty() ) continue;
            int num = itr->second.unit_num;
            int parent_num = uim[itr->second.parent_unit].unit_num;
            if( parent_num >= num )
            {
                itr->second.unit_num = parent_num + 1;
                hasMoreToRevise = true;
            }
        }
        if( !hasMoreToRevise ) break;
    }
}

void print( UnitInfoMap& uim )
{
    cout << "节点\t父节点\t层次号\n";
    for( UnitInfoMap::iterator itr = uim.begin(); itr != uim.end(); itr++ )
    {
        cout << itr->first << "\t"
             << itr->second.parent_unit << "\t"
             << itr->second.unit_num << endl;
    }
}

int main()
{
    UnitInfoMap uim;
    initUninInfoMap( uim ); // 初始化

    caclUnitInfoMap( uim ); // 计算层次号

    print( uim ); // 打印
    return 0;
}
