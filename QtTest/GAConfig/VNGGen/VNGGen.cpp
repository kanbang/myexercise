#include <Windows.h>
#include "VNGGen.h"
#include "Tool.h"
#include "GraphParser.h"

#include <fstream>
#include <sstream>

#define DATA_FILE_NAME "data.txt"
#define GV_FILE_NAME "gd.gv"
#define OUTPUT_FILE_NAME "gd.txt"

#define PI 3.1415926535897932384626433832795

#define BOOL_2_STRING(b) (b?"true":"false")

enum GRAPH_INDEX
{
    EDGE_INDEX       = 0,  // ��֧���
    START_NODE_INDEX = 1,  // ʼ�ڵ���
    END_NODE_INDEX   = 2,  // ĩ�ڵ���
    DEF_COL_NUM      = 3   // ÿһ�е�����
};

// �������������ļ�
// ���ڱ�ŵ�ʵ�ֻ���Ҫ�޸�
// ĿǰĬ��ʹ��lemon�Դ���id����
static bool CreateTopologyDataFile( Digraph& dg, const string& dataFilePath )
{
    // 1) �����ļ�
    ofstream outFile( dataFilePath.c_str() );
    if( !outFile ) return false;

    // 2) ʹ��lemon�Դ���id��Ϊ���
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        Digraph::Node u = dg.source( e );
        Digraph::Node v = dg.target( e );

        outFile << dg.id( e ) << "\t" << dg.id( u ) << "\t" << dg.id( v ) << "\n";
    }

    outFile.close();

    return true;
}

// ע�⣺
//       ��������λ:Ӣ��
static bool CreateDotFile( const string& dataFilePath, const string& gvFilePath,
                           double m_nodesep, double m_ranksep,
                           double m_graphRatio , double m_graphWidth, double m_graphHeight, bool m_useDefWH,
                           double m_nodeWidth, double m_nodeHeight, double m_nodeTextHeight, double m_edgeTextHeight,
                           bool splines = true )
{
    ifstream inFile( dataFilePath.c_str() );
    if( !inFile ) return false;

    ofstream outFile( gvFilePath.c_str() );
    if( !outFile ) return false;

    bool ret = true;

    /* д��ͼ�����������Լ�ͼ������G */
    outFile << "digraph G {\n";

    // ����ͼ������
    if( m_useDefWH )
    {
        outFile << "\tgraph[rankdir=BT, splines=" << BOOL_2_STRING( splines )
                << ", nodesep=\"" << m_nodesep << "\", ranksep=\"" << m_ranksep << "\"]\n";
    }
    else
    {
        outFile << "\tgraph[rankdir=BT, splines=" << BOOL_2_STRING( splines )
                << ", nodesep=\"" << m_nodesep << "\", ranksep=\"" << m_ranksep << "\""
                << ", ratio=fill, size=\"" << m_graphWidth
                << "," << m_graphHeight << "\"]\n";
    }

    // ���ýڵ�����
    outFile << "\tnode[width=\"" << m_nodeWidth << "\", height=\"" << m_nodeHeight << "\", fontsize=\"" << m_nodeTextHeight << "\"]\n";

    // ���÷�֧����
    outFile << "\tedge[arrowhead=none, arrowtail=none, fontsize=\"" << m_edgeTextHeight << "\"];\n";

    string strLine;
    while( getline( inFile, strLine ) )
    {
        StringArray values;
        SplitString( strLine, " \t", values );
        if( values.size() != DEF_COL_NUM )
        {
            ret = false;
            break;
        }

        string dotLine;
        /*
         * values[0] <==> ��֧���
         * values[1] <==> ʼ�ڵ�
         * values[2] <==> ĩ�ڵ�
         */
        string sNode = values[START_NODE_INDEX];
        string tNode = values[END_NODE_INDEX];
        string edgeNum = values[EDGE_INDEX];
        outFile << "\tv" << sNode << "->v" << tNode << " [label=\"e" << edgeNum << "\"];\n";
        //dotLine.Format(_T("\t%s->%s [label=\"%s\"];\n"), sNode, tNode, edgeNum);
    }
    outFile << "}\n";

    outFile.close();
    inFile.close();

    return ret;
}

static bool GenDrawFormatFile( const string& dotExePath, const string& fmt, const string& gvFilePath, const string& outFilePath )
{
    /*
     * �����ⲿ����dot.exe
     * ִ�����dot -Tplain -o gd.dat gd.gv
     *            �� dot -Tsvg -o gd.dat gd.gv
     * ʾ��������һ��pngͼ��==> dot -Tpng -o xx.png gd.gv
     * �μ�Graphviz���ĵ�
     */
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset( &si, 0, sizeof( si ) );
    si.cb = sizeof( si );
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW;

    stringstream ss;
    ss << " -T" << fmt << " -o " << outFilePath << " " << gvFilePath;
    string cmdLine = ss.str();

    /*
     * CreateProcessֻ�ǳ�ʼ���������̣�����ִ����Ҫһ��ʱ��
     * �����ʹ��WaitForSingleObject�����ȴ�����ִ�н���
     * ��ôGraphParser���޷���gd.gv�ļ���
     * ��Ϊdot����һЩ�Ƚϴ���ļ�����Ҳ����Ҫһ��ʱ��ģ���������������
     */
    BOOL ret = CreateProcess( ( char* )dotExePath.c_str(), ( char* )cmdLine.c_str(), NULL, FALSE, NULL, NULL, NULL, NULL, &si, &pi );
    if( ret )
    {
        WaitForSingleObject( pi.hProcess, INFINITE );
        CloseHandle( pi.hThread );
        CloseHandle( pi.hProcess );
    }
    return ret;
}

static bool GenVentNetworkGraph( Digraph& dg, const string& dotExePath,
                                 double m_nodeSep, double m_rankSep,
                                 double m_graphRatio, double m_graphWidth, double m_graphHeight, bool m_useDefWH,
                                 double m_nodeWidth, double m_nodeHeight, double m_nodeTextHeight, double m_edgeTextHeight,
                                 GraphInfo& gi, NodeInfoArray& nis, EdgeInfoArray& eis )
{
    // 1) �������������ļ�·��
    // a) ��ȡ��ǰģ��·��
    string dirPath = GetAppPathDir();

    //eo::log<<"��ǰģ��·��:"<<dirPath<<std::endl;

    // b) �������������ļ�·��
    string dataFilePath = BuildPath( dirPath, DATA_FILE_NAME );

    // c) ����*.gv�ļ���·��
    string gvFilePath = BuildPath( dirPath, GV_FILE_NAME );

    // d) ��������ļ���·��
    string outFilePath = BuildPath( dirPath, OUTPUT_FILE_NAME );

    //eo::log<<"���������ļ�:"<<dataFilePath<<std::endl;
    //eo::log<<"gv�ļ���·��:"<<gvFilePath<<std::endl;
    //eo::log<<"����ļ���·��:"<<outFilePath<<std::endl;

    // 2) �������������ļ�
    if( !CreateTopologyDataFile( dg, dataFilePath ) )
    {
        RemoveFile( dataFilePath );
        //eo::log<<"������������ʧ��"<<std::endl;
        return false;
    }

    // 3) ����gv�ļ�
    // if(!CreateDotFile(dataFilePath, gvFilePath, 0.5, 0.75, false))  // �����߻��Ʒ�֧
    if( !CreateDotFile( dataFilePath, gvFilePath,
                        m_nodeSep, m_rankSep,
                        m_graphRatio , m_graphWidth , m_graphHeight, m_useDefWH,
                        m_nodeWidth, m_nodeHeight, m_nodeTextHeight, m_edgeTextHeight,
                        true ) ) // ���������߻��Ʒ�֧
    {
        //eo::log<<"���������ļ���ʽ����ȷ!"<<std::endl;
        // ɾ����ʱ�ļ�
        RemoveFile( dataFilePath );
        RemoveFile( gvFilePath );
        return false;
    }

    // 4) ����dot.exe��������ļ����ļ���ʽΪplain
    string fmt = "plain";
    if( !GenDrawFormatFile( dotExePath, fmt, gvFilePath, outFilePath ) )
    {
        //eo::log<<"�������̵���dotʧ��!"<<std::endl;
        // ɾ����ʱ�ļ�
        RemoveFile( dataFilePath );
        RemoveFile( gvFilePath );
        RemoveFile( outFilePath );
        return false;
    }

    // 5) ����plain��ʽ����ļ�
    //    ��ȡ�ڵ�ͷ�֧�ļ���������Ϣ
    bool ret = GraphParser::ReadGraphInfoFromPlainFile( outFilePath, gi, nis, eis );
    if( !ret )
    {
        ClearNodeInfoArray( nis );
        ClearEdgeInfoArray( eis );

        //eo::log<<"����ʧ��!"<<std::endl;
    }

    // ɾ����ʱ�ļ�
    RemoveFile( dataFilePath );
    RemoveFile( gvFilePath );
    RemoveFile( outFilePath );

    //eo::log<<"����"<<BOOL_2_STRING(ret)<<std::endl;

    return ret;
}

#include <fstream>

bool VNGGen( Digraph& dg, VNGParam& param, GraphInfo& gi, NodeInfoArray& nis, EdgeInfoArray& eis )
{
    //using namespace  std;
    //ofstream outfile("d:\\log.txt");

    // ��ʼ��log
    //eo::log << eo::file(BuildPath(GetAppPathDir(), "vng_log.txt"));
    //eo::log << eo::file("d:\\vng_log.txt");

    // 1) ����graphviz·��
    string dotExePath;
    if( !ReadGraphvizInstallPath( dotExePath ) )
    {
        //eo::log<<"û���ҵ�gv\n";
        //outfile<<"û���ҵ�gv\n";
        //outfile.close();
        return false;
    }

    //outfile<<"gv·��:"<<dotExePath<<std::endl;

    // 2) ����dot.exe��������ͼ����ȡ������Ϣ
    // ע���Ӵʵ��ȡ�����ݵ�λΪmm������������ĵ�λΪinch��
    //      ʹ��INCH_2_MM�����ת��
    bool ret = GenVentNetworkGraph( dg, dotExePath,
                                    MM_2_INCH( param.m_nodeSep ), MM_2_INCH( param.m_rankSep ),
                                    MM_2_INCH( param.m_graphRatio ), MM_2_INCH( param.m_graphWidth ),
                                    MM_2_INCH( param.m_graphHeight ), param.m_useDefWH,
                                    MM_2_INCH( param.m_nodeWidth ), MM_2_INCH( param.m_nodeHeight ),
                                    MM_2_INCH( param.m_nodeTextHeight ), MM_2_INCH( param.m_edgeTextHeight ),
                                    gi, nis, eis );

    //if(ret)
    //{
    //	outfile<<"����ɹ�"<<std::endl;
    //}
    //else
    //{
    //	outfile<<"����shibai"<<std::endl;
    //}
    //outfile.close();

    return ret;
}

void ClearNodeInfoArray( NodeInfoArray& nis )
{
    GraphParser::ClearNodeInfoArrayImpl( nis );
}

void ClearEdgeInfoArray( EdgeInfoArray& eis )
{
    GraphParser::ClearEdgeInfoArrayImpl( eis );
}