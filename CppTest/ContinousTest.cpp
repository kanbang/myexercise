#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef vector<int> IntArray;

void FindContinuousEdges( int n, const IntArray& edges, IntArray& con_edges )
{
    // 分析数组，计算连续部分的长度
    IntArray con_edges_len_array;

    int pos = edges[0];
    int count = 1;
    for( int i = 1; i < edges.size(); i++ )
    {
        cout << "pos=" << pos << " edges[" << i << "]=" << edges[i] << endl;
        if( edges[i] == ( pos + 1 ) % n )
        {
            count++;
        }
        else
        {
            con_edges_len_array.push_back( count );
            count = 1;
        }
        pos = edges[i];
    }
    con_edges_len_array.push_back( count );

    cout << "---------------------------------------" << endl;
    for( int i = 0; i < con_edges_len_array.size(); i++ )
    {
        cout << con_edges_len_array[i] << " ";
    }
    cout << "\n---------------------------------------" << endl;

    int k = max_element( con_edges_len_array.begin(), con_edges_len_array.end() ) - con_edges_len_array.begin();

    cout << "k=" << k << endl;

    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += con_edges_len_array[i];
    }
    int t = s + con_edges_len_array[k];
    for( int i = s; i < t; i++ )
    {
        con_edges.push_back( edges[i] );
    }
}

int main()
{
    int n = 15;
    IntArray edges;
    edges.push_back( 0 );
    edges.push_back( 3 );
    edges.push_back( 4 );
    edges.push_back( 5 );
    edges.push_back( 8 );
    edges.push_back( 9 );
    edges.push_back( 10 );
    edges.push_back( 13 );

    IntArray con_edges;
    FindContinuousEdges( n, edges, con_edges );

    cout << "打印查找的连续边" << endl;
    for( int i = 0; i < con_edges.size(); i++ )
    {
        cout << con_edges[i] << " ";
    }
    cout << endl;
    return 0;
}
