#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void f()
{
    string inFilePath = "xxx.txt";
    string outFilePath = "rw.txt";

    ifstream inFile( inFilePath.c_str() );
    if( !inFile )
    {
        cout << "打开错误1" << endl;
        return;
    }

    ofstream outFile( outFilePath.c_str() );
    if( !outFile )
    {
        cout << "打开错误1" << endl;
        return;
    }

    int lineNum;
    inFile >> lineNum; // 读取列数
    cout << "行数:" << lineNum << endl;

    while( !inFile.eof() )
    {
        string value;
        for( int i = 0; i < lineNum; i++ )
        {
            inFile >> value;
//			cout<<"值:"<<value<<endl;
            outFile << value;
            if( i < lineNum - 1 )
            {
                outFile << "\t";
            }
        }
        outFile << endl;
    }

    inFile.close();
    outFile.close();
}

int main()
{
    f();
    return 0;
}
