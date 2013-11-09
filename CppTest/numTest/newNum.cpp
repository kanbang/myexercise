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
        // 打开文件失败
        //cout<<"打开输入txt文件失败\n";
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
        // 读取始末节点
        string s, t;
        infile >> s >> t;

        // 排除空字符串
        if( s.empty() || t.empty() ) continue;

        //cout<<s.length()<<"\t"<<t.length()<<endl;
        //cout<<s<<"*"<<"\t"<<t<<"*"<<endl;

        // 转换成大写
        transform( s.begin(), s.end(), s.begin(), toupper );
        transform( t.begin(), t.end(), t.begin(), toupper );

        // 排除NULL字符串
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
        //cout<<"输入数据文件格式错误！\n必须要保证每行有2列数据!\n";
        return;
    }

    ofstream outfile( outFile.c_str() );
    if( !outfile )
    {
        // 创建输出文件失败
        outfile.close();
        return;
    }

    int len = sNodes.size();
    for( int i = 0; i < len; i++ )
    {
        // 分支编号   始节点编号  末节点编号
        outfile << i + 1 << "\t" << sNodes[i] << "\t" << tNodes[i] << endl;
    }

    outfile << "\n\n\n";
    outfile << "*******************************************\n";
    outfile << "************ 下面的部分可以删除 ***********\n";
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
        cout << "使用说明: newNum.exe inData outData\n"
             << "    (1) inData  -- 输入txt文件\n"
             << "    (2) outData -- 输出txt文件\n"
             << "例如：newNum xx.txt yy.txt\n";
        return 0;
    }

    //将全局区域设为操作系统默认区域，以支持中文路径
    locale::global( std::locale( "" ) );

    // 重新编号
    ChangeNewNum( argv[1], argv[2] );

    //恢复全局locale，如果不恢复，可能导致cout无法输出中文
    setlocale( LC_ALL, "C" );

    return 0;
}
