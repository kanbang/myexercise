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
        cout << "�򿪴���1" << endl;
        return;
    }

    ofstream outFile( outFilePath.c_str() );
    if( !outFile )
    {
        cout << "�򿪴���1" << endl;
        return;
    }

    int lineNum;
    inFile >> lineNum; // ��ȡ����
    cout << "����:" << lineNum << endl;

    while( !inFile.eof() )
    {
        string value;
        for( int i = 0; i < lineNum; i++ )
        {
            inFile >> value;
//			cout<<"ֵ:"<<value<<endl;
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
