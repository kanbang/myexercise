#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

void ChangeNewNum( const string& inFile, const string& outFile )
{
    ifstream infile( inFile.c_str() );
    if( !infile )
    {
        // ���ļ�ʧ��
        //cout<<"������txt�ļ�ʧ��\n";
        infile.close();
        return;
    }

    typedef vector<string> Nodes;
    typedef vector<int> IntNodes;
    Nodes nodes;
    Nodes nodes1, nodes2;
    IntNodes sNodes, tNodes;
    while( !infile.eof() )
    {
        // ��ȡʼĩ�ڵ�
        string s, t;
        infile >> s >> t;

        // �ų����ַ���
        if( s.empty() || t.empty() ) continue;

        //cout<<s.length()<<"\t"<<t.length()<<endl;
        //cout<<s<<"*"<<"\t"<<t<<"*"<<endl;

        // ת���ɴ�д
        transform( s.begin(), s.end(), s.begin(), toupper );
        transform( t.begin(), t.end(), t.begin(), toupper );

        // �ų�NULL�ַ���
        if( s == "NULL" || t == "NULL" ) continue;

        nodes1.push_back( s );
        nodes2.push_back( t );
        Nodes::iterator itr = find( nodes.begin(), nodes.end(), s );
        sNodes.push_back( itr - nodes.begin() );
        if( itr == nodes.end() )
        {
            nodes.push_back( s );
        }

        itr = find( nodes.begin(), nodes.end(), t );
        tNodes.push_back( itr - nodes.begin() );
        if( itr == nodes.end() )
        {
            nodes.push_back( t );
        }
    }
    infile.close();

    if( sNodes.size() != tNodes.size() )
    {
        //cout<<"���������ļ���ʽ����\n����Ҫ��֤ÿ����2������!\n";
        return;
    }

    ofstream outfile( outFile.c_str() );
    if( !outfile )
    {
        // ��������ļ�ʧ��
        outfile.close();
        return;
    }

    int len = sNodes.size();
    for( int i = 0; i < len; i++ )
    {
        // ��֧���   ʼ�ڵ���  ĩ�ڵ���
        outfile << i + 1 << "\t" << sNodes[i] << "\t" << tNodes[i] << endl;
    }

    outfile << "\n\n\n";
    outfile << "*******************************************\n";
    outfile << "************ ����Ĳ��ֿ���ɾ�� ***********\n";
    outfile << "*******************************************\n";
    outfile << "\n";
    int len2 = nodes1.size();
    for( int i = 0; i < len2; i++ )
    {
        outfile << nodes1[i] << "\t" << nodes2[i] << endl;
    }
    outfile.close();
}

int main( int argc, char* argv[] )
{
    //cout<<argv[1]<<endl<<argv[2]<<endl;
    if( argc != 3 )
    {
        cout << "ʹ��˵��: newNum.exe inData outData\n"
             << "    (1) inData  -- ����txt�ļ�\n"
             << "    (2) outData -- ���txt�ļ�\n"
             << "���磺newNum xx.txt yy.txt\n";
        return 0;
    }

    //��ȫ��������Ϊ����ϵͳĬ��������֧������·��
    locale::global( std::locale( "" ) );

    // ���±��
    ChangeNewNum( argv[1], argv[2] );

    //�ָ�ȫ��locale��������ָ������ܵ���cout�޷��������
    setlocale( LC_ALL, "C" );

    return 0;
}
