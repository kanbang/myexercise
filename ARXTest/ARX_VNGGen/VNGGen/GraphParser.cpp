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
    // 至少需要4个数据
    if( values.size() < 4 )
    {
        //acutPrintf(_T("\ngraph所在行数据少于4个"));
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
    //  至少需要7个数据
    if( values.size() < 7 )
    {
        //acutPrintf(_T("\nnode所在行数据少于7个"));
        return false;
    }
    /* 数据格式比较简单，因此name和label是相同的 */
    // ni->name = values[1];
    ni->x = INCH_2_MM( StringToDouble( values[2] ) );
    ni->y = INCH_2_MM( StringToDouble( values[3] ) );
    ni->cx = INCH_2_MM( StringToDouble( values[4] ) );
    ni->cy = INCH_2_MM( StringToDouble( values[5] ) );
    ni->id = StringToInt( RemovePrefix( values[6] ) ); // 去掉前缀v，并转换成整数id
    return true;
}

static bool ReadEdgeInfo( const StringArray& values, EdgeInfo* ei )
{
    //  至少需要4个数据
    if( values.size() < 4 )
    {
        //acutPrintf(_T("\nedge所在行数据少于4个"));
        return false;
    }
    ei->sn = StringToInt( RemovePrefix( values[1] ) );
    ei->tn = StringToInt( RemovePrefix( values[2] ) );
    int n = StringToInt( values[3] ); // 控制点个数
    /* 前面4个数据 + 2*n个控制点数据 + 3个分支标签数据*/
    if( ( int )values.size() < ( 7 + 2 * n ) )
    {
        //acutPrintf(_T("\n控制点个数:%d，且node所在行数据少于%d个"), n, (7+2*n));
        return false;
    }
    /* 读取控制点数据 */
    for( int i = 0; i < n; i++ )
    {
        int pos = 4 + 2 * i;
        double x = INCH_2_MM( StringToDouble( values[pos] ) );
        double y = INCH_2_MM( StringToDouble( values[pos + 1] ) );
        ei->cnx.push_back( x );
        ei->cny.push_back( y );
    }

    /* 读取文字标签数据 */
    int curPos = 4 + 2 * n;
    ei->id = StringToInt( RemovePrefix( values[curPos] ) ); // 去掉前缀e,并转换成整数id
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
            //acutPrintf(_T("\n读取一行:%s"), strLine);
            if( IsStop( strLine ) )
            {
                //acutPrintf(_T("\n发现文件结束标记：stop!"));
                break;
            }

            StringArray values;
            SplitString( strLine, " \t", values );
            if( values.empty() ) continue;

            string tag = values[0];  // 标签
            //acutPrintf(_T("\n==>标签:%s"), tag);
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