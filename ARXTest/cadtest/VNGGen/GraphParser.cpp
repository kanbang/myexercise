#include "GraphParser.h"
#include "Tool.h"

#include <fstream>

static bool IsGraph( const string& tag )
{
    return ( tag == "graph" );
}

static bool IsNode( const string& tag )
{
    return ( tag == "node" );
}

static bool IsEdge( const string& tag )
{
    return ( tag == "edge" );
}

static bool IsStop( const string& strLine )
{
    return ( strLine == "stop" );
}

static bool ReadGraphInfo( const StringArray& values, GraphInfo* gi )
{
    // ������Ҫ4������
    if( values.size() < 4 )
    {
        //acutPrintf(_T("\ngraph��������������4��"));
        return false;
    }
    gi->ratio = StringToDouble( values[1] );
    gi->width = INCH_2_MM( StringToDouble( values[2] ) );
    gi->height = INCH_2_MM( StringToDouble( values[3] ) );
    return true;
}

static string RemovePrefix( const string& str )
{
    return str.substr( 1 );
}

static bool ReadNodeInfo( const StringArray& values, NodeInfo* ni )
{
    //  ������Ҫ7������
    if( values.size() < 7 )
    {
        //acutPrintf(_T("\nnode��������������7��"));
        return false;
    }
    /* ���ݸ�ʽ�Ƚϼ򵥣����name��label����ͬ�� */
    // ni->name = values[1];
    ni->x = INCH_2_MM( StringToDouble( values[2] ) );
    ni->y = INCH_2_MM( StringToDouble( values[3] ) );
    ni->cx = INCH_2_MM( StringToDouble( values[4] ) );
    ni->cy = INCH_2_MM( StringToDouble( values[5] ) );
    ni->id = StringToInt( RemovePrefix( values[6] ) ); // ȥ��ǰ׺v����ת��������id
    return true;
}

static bool ReadEdgeInfo( const StringArray& values, EdgeInfo* ei )
{
    //  ������Ҫ4������
    if( values.size() < 4 )
    {
        //acutPrintf(_T("\nedge��������������4��"));
        return false;
    }
    ei->sn = StringToInt( RemovePrefix( values[1] ) );
    ei->tn = StringToInt( RemovePrefix( values[2] ) );
    int n = StringToInt( values[3] ); // ���Ƶ����
    /* ǰ��4������ + 2*n�����Ƶ����� + 3����֧��ǩ����*/
    if( ( int )values.size() < ( 7 + 2 * n ) )
    {
        //acutPrintf(_T("\n���Ƶ����:%d����node��������������%d��"), n, (7+2*n));
        return false;
    }
    /* ��ȡ���Ƶ����� */
    for( int i = 0; i < n; i++ )
    {
        int pos = 4 + 2 * i;
        double x = INCH_2_MM( StringToDouble( values[pos] ) );
        double y = INCH_2_MM( StringToDouble( values[pos + 1] ) );
        ei->cnx.push_back( x );
        ei->cny.push_back( y );
    }

    /* ��ȡ���ֱ�ǩ���� */
    int curPos = 4 + 2 * n;
    ei->id = StringToInt( RemovePrefix( values[curPos] ) ); // ȥ��ǰ׺e,��ת��������id
    ei->tx = INCH_2_MM( StringToDouble( values[curPos + 1] ) );
    ei->ty = INCH_2_MM( StringToDouble( values[curPos + 2] ) );

    return true;
}

namespace GraphParser
{
    bool ReadGraphInfoFromPlainFile( const string& outFilePath, GraphInfo& gi, NodeInfoArray& nis, EdgeInfoArray& eis )
    {
        ifstream inFile( outFilePath.c_str() );
        if( !inFile ) return false;

        bool ret = true;
        string strLine;
        while( getline( inFile, strLine ) && ret )
        {
            //acutPrintf(_T("\n��ȡһ��:%s"), strLine);
            if( IsStop( strLine ) )
            {
                //acutPrintf(_T("\n�����ļ�������ǣ�stop!"));
                break;
            }

            StringArray values;
            SplitString( strLine, " \t", values );
            if( values.empty() ) continue;

            string tag = values[0];  // ��ǩ
            //acutPrintf(_T("\n==>��ǩ:%s"), tag);
            if( IsGraph( tag ) )
            {
                ret = ReadGraphInfo( values, &gi );
            }
            else if( IsNode( tag ) )
            {
                NodeInfo* ni = new NodeInfo();
                ret = ReadNodeInfo( values, ni );
                if( ret ) nis.push_back( ni );
            }
            else if( IsEdge( tag ) )
            {
                EdgeInfo* ei = new EdgeInfo();
                ret = ReadEdgeInfo( values, ei );
                if( ret ) eis.push_back( ei );
            }
        }
        inFile.close();

        return ret;
    }

    void ClearNodeInfoArrayImpl( NodeInfoArray& nis )
    {
        NodeInfoArray::iterator itr;
        for( itr = nis.begin(); itr != nis.end(); itr++ )
        {
            delete *itr;
        }
        nis.clear();
    }

    void ClearEdgeInfoArrayImpl( EdgeInfoArray& eis )
    {
        EdgeInfoArray::iterator itr;
        for( itr = eis.begin(); itr != eis.end(); itr++ )
        {
            delete *itr;
        }
        eis.clear();
    }
}