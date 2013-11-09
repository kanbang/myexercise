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
    uim.insert( UnitInfoMap::value_type( "�й�", info ) );

    info.parent_unit = "�й�";
    uim.insert( UnitInfoMap::value_type( "�人", info ) );

    info.parent_unit = "�й�";
    uim.insert( UnitInfoMap::value_type( "����", info ) );

    info.parent_unit = "�人";
    uim.insert( UnitInfoMap::value_type( "��ɽ", info ) );
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
    cout << "�ڵ�\t���ڵ�\t��κ�\n";
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
    initUninInfoMap( uim ); // ��ʼ��

    caclUnitInfoMap( uim ); // �����κ�

    print( uim ); // ��ӡ
    return 0;
}
