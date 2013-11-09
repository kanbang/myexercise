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
        cout << "�򿪴���1" << endl;
        return;
    }

    ofstream outFile( outFilePath.c_str() );
    if( !outFile )
    {
        cout << "�򿪴���2" << endl;
        return;
    }

    int lineNum;
    inFile >> lineNum; // ��ȡ����
    cout << "����:" << lineNum << endl;

    vector<string> edges;
    vector<string> degrees;

    // ��ȡ��֧���
    string value;
    for( int i = 0; i < lineNum; i++ )
    {
        inFile >> value;
        edges.push_back( value );
    }

    // ��ȡӰ���
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
