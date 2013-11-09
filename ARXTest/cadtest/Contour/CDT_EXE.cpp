// 参考
// http://www.cs.cmu.edu/~quake/triangle.html
// http://en.wikipedia.org/wiki/Delaunay_triangulation
// http://www.cgal.org/Manual/latest/doc_html/cgal_manual/Triangulation_2/Chapter_main.html#Section_36.7
#include <Windows.h>

#include "Contour.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

#define TAB "\t"
#define IN_POLY_FILE_NAME "test"
#define OUT_POLY_FILE_NAME "test.1"
#define MODULE_NAME "Contour.dll"

string GetAppPathDir()
{
    char szMoudlePath[_MAX_PATH];
    GetModuleFileName( GetModuleHandle( MODULE_NAME ), szMoudlePath, _MAX_PATH );

    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    _splitpath( szMoudlePath, drive, dir, NULL, NULL );

    char path[_MAX_PATH];
    _makepath( path, drive, dir, NULL, NULL );

    return string( path );
}

static void CreatePolyFile( const string& polyFilePath,
                            const PointArray& pts,
                            const EdgeArray& segments,
                            const PointArray& holes )
{
    ofstream outFile( polyFilePath.c_str() );
    // 设置精度为小数点5位，输出时按照固定精度输出
    // 不使用科学计数法输出
    outFile << std::setprecision( 5 ) << std::fixed;

    // 写入第1节(point section)
    outFile << pts.size() << TAB
            << 2 << TAB
            << 0 << TAB
            << 0 << endl;

    // 写入点坐标
    for( int i = 0; i < ( int )pts.size(); i++ )
    {
        outFile << i << TAB
                << pts[i].x << TAB
                << pts[i].y << endl;
    }

    // 写入第2节(segment section)
    outFile << segments.size() << endl;
    for( int i = 0; i < ( int )segments.size(); i++ )
    {
        outFile << i << TAB
                << segments[i].s << TAB
                << segments[i].t << endl;
    }

    // 写入第3节(hole section)
    outFile << holes.size() << endl;
    for( int i = 0; i < ( int )holes.size(); i++ )
    {
        outFile << i << TAB
                << holes[i].x << TAB
                << holes[i].y << endl;
    }

    outFile.close();
}

static bool GenTriangle( const string& triangleExePath, const string& path )
{
    // 格式化命令参数
    // 例如：-pe_Q xx.poly
    stringstream ss;
    ss << " -pcez_Q " << path;
    string cmdLine = ss.str();

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset( &si, 0, sizeof( si ) );
    si.cb = sizeof( si );
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW;

    BOOL ret = CreateProcess( triangleExePath.c_str(), ( char* )cmdLine.c_str(), NULL, FALSE, NULL, NULL, NULL, NULL, &si, &pi );
    if( ret )
    {
        WaitForSingleObject( pi.hProcess, INFINITE );
        CloseHandle( pi.hThread );
        CloseHandle( pi.hProcess );
    }
    return ( ret == TRUE );
}

static void ReadEdges( const string& edgeFilePath, EdgeArray& ea )
{
    ifstream inFile;
    inFile.open( edgeFilePath.c_str() );

    int ne, b;
    inFile >> ne >> b;

    for( int i = 0; i < ne; i++ )
    {
        int ei;
        inFile >> ei;

        int s, t, bb;
        inFile >> s >> t >> bb;

        DT_Edge e = {s, t};
        ea.push_back( e );
    }
    inFile.close();
}

static void ReadTriangles( const string& triangleFilePath, TriangleArray& ta )
{
    ifstream inFile;
    inFile.open( triangleFilePath.c_str() );

    int nt, v, b;
    inFile >> nt >> v >> b;

    for( int i = 0; i < nt; i++ )
    {
        int ti;
        inFile >> ti;

        int t1, t2, t3;
        inFile >> t1 >> t2 >> t3;

        DT_Triangle t = {t1, t2, t3};
        ta.push_back( t );

        if( b != 0 )
        {
            int bb;
            inFile >> bb;
        }
    }
    inFile.close();
}

static void ParseOutPolyFile( const string& path, TriangleArray& ta, EdgeArray& ea )
{
    // 拆分路径
    char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _splitpath( path.c_str(), drive, dir, fname, ext );

    stringstream ss;
    ss << drive << dir << fname << ".1.edge";
    ReadEdges( ss.str(), ea );

    ss.clear();
    ss << drive << dir << fname << ".1.ele";
    ReadTriangles( ss.str(), ta );
}

static string FormatFilePath( const string& dir, const string& name, const string& ext )
{
    stringstream ss;
    ss << dir << name << ext;
    return ss.str();
}

void CDT_EXE(
    const PointArray& pts,
    const EdgeArray& segments,
    const PointArray& holes,
    EdgeArray& ea,
    TriangleArray& ta )
{
    // 1) 创建.poly文件
    string dir = GetAppPathDir();
    string polyFilePath = FormatFilePath( dir, IN_POLY_FILE_NAME, ".poly" );
    CreatePolyFile( polyFilePath, pts, segments, holes );

    // 2) 调用triangle.exe
    GenTriangle( FormatFilePath( dir, "triangle", ".exe" ), polyFilePath );

    // 3) 读取计算得到的边和三角形
    ReadEdges( FormatFilePath( dir, OUT_POLY_FILE_NAME, ".edge" ), ea );
    ReadTriangles( FormatFilePath( dir, OUT_POLY_FILE_NAME, ".ele" ), ta );
}
