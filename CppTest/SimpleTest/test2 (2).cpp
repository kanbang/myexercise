#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void f()
{
    string inFilePath = "zzz.txt";
    string outFilePath = "rw3.txt";

    ifstream inFile( inFilePath.c_str() );
    if( !inFile )
    {
        cout << "打开错误1" << endl;
        return;
    }

    ofstream outFile( outFilePath.c_str() );
    if( !outFile )
    {
        cout << "打开错误2" << endl;
        return;
    }

    int lineNum;
    inFile >> lineNum; // 读取列数
    cout << "行数:" << lineNum << endl;

    vector<string> edges;
    vector<string> degrees;

    // 读取分支编号
    string value;
    for( int i = 0; i < lineNum; i++ )
    {
        inFile >> value;
        edges.push_back( value );
    }

    // 读取影响度
    for( int i = 0; i < lineNum; i++ )
    {
        inFile >> value;
        degrees.push_back( value );
    }
    inFile.close();
    for( int i = 0; i < lineNum; i++ )
    {
        outFile << degrees[i] << "\t" << edges[i] << endl;
    }
    outFile.close();
}

int main()
{
    f();
    return 0;
}
