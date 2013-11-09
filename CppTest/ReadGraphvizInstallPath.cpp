#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// 读取graphviz安装路径
static bool ReadGraphvizInstallPath( string& path )
{
    long lRet = 0;
    string strRegGV = "SOFTWARE\\AT&T Research Labs\\Graphviz"; // graphviz
    HKEY   hResult;
    lRet = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, strRegGV.c_str(), 0, KEY_QUERY_VALUE, &hResult );
    if( lRet == ERROR_SUCCESS )
    {
        DWORD      WordType =  REG_SZ;
        DWORD      WordData =  100;
        char     KeyByte[1024];
        lRet = ::RegQueryValueEx( hResult, ( "InstallPath" ), NULL, &WordType, ( LPBYTE )KeyByte, &WordData );
        if( lRet == ERROR_SUCCESS )
        {
            path = KeyByte;
            stringstream ss;
            ss << path << "bin\\dot.exe";
            path = ss.str();
        }
        RegCloseKey( hResult );
    }
    return ( lRet == ERROR_SUCCESS );
}

int main()
{
    string path;
    if( ReadGraphvizInstallPath( path ) )
    {
        cout << path << endl;
    }

    return 0;
}
