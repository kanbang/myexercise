#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void WriteDotFile( const string& inFile, bool needEdgeNum )
{
    ifstream infile( inFile.c_str() );
    if( !infile )
    {
        // ���ļ�ʧ��
        infile.close();
        return ;
    }

    ofstream outfile( "gd.gv" );
    // д��ͼ�����������Լ�ͼ������G
    outfile << "digraph G {\n";
    // ͨ��ratio��size��������ͼ�εı���
    // �����ڲ����з���size��̫��ȷ�������׵���dot�ҵ�
    // ��ʱֻ��ratio����ratio=height/widht
    //outfile<<"graph[rankdir=BT, ratio=1.333];\n";
    outfile << "\tgraph[rankdir=BT];\n";

    while( !infile.eof() )
    {
        int edge, start_node, end_node;
        infile >> edge >> start_node >> end_node;
        outfile << "\t"
                << "v" << start_node
                << "->"
                << "v" << end_node;

        if( needEdgeNum )
        {
            outfile << " [label=\"e" << edge << "\"]";
        }

        outfile << ";\n";
    }

    outfile << "}";
    outfile.close();
    infile.close();
}

int main( int argc, char* argv[] )
{
    if( argc != 3 )
    {
        cout << "ʹ��˵��: gvFileGen.exe graphFile [-e]\n"
             << "    (1) graphFile -- ��������txt�ļ�\n"
             << "    (2) e         -- ��Ҫ��ʾ��֧���\n"
             << "        ����û�������ò�������ôĬ������²���ʾ��֧���";
        return 0;
    }
    //��ȫ��������Ϊ����ϵͳĬ��������֧������·��
    locale::global( std::locale( "" ) );

    // ����dot�ļ�
    WriteDotFile( argv[1], strcmp( argv[2], "-e" ) == 0 );

    //�ָ�ȫ��locale��������ָ������ܵ���cout�޷��������
    setlocale( LC_ALL, "C" );

    return 0;
}