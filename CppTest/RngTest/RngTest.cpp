#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
typedef vector<int> IntArray;

#include <eo>
//#pragma comment(lib, "es.lib")
//#pragma comment(lib, "ga.lib")
//#pragma comment(lib, "eo.lib")
#pragma comment(lib, "eoutils.lib")

/* eo文件中包含一个rng全局对象
 * extern eoRng rng;
 * */

/* 随机数固定种子 */
const unsigned long SEED = 42;
/* 整数区间,例如[0, 6) ==> [0, 5] */
const int DATA_START = 0;
const int DATA_END = 100;
const int DATA_NUM = DATA_END - DATA_START;
/* 随机数测试运行次数 */
const int RUN_COUNT = 100;

int useEo = 0;

// 使用eo
void eo_rng_gen( int start, int end, int num, IntArray& datas )
{
    // 生成随机数
    for( int i = 0; i < num; i++ )
    {
        datas.push_back( ( int )rng.uniform( start, end ) + 1 );
    }
}

void eo_rng_seed( unsigned long seed )
{
    rng.reseed( seed );
}

// 使用crt自带的rand
void sys_rng_gen( int start, int end, int num, IntArray& datas )
{
    // 生成随机数
    for( int i = 0; i < num; i++ )
    {
        int u = ( double )rand() / ( RAND_MAX + 1 ) * ( end - start ) + start;
        datas.push_back( u + 1 );
    }
}

void sys_rng_seed( unsigned long seed )
{
    srand( seed );
}

void rng_gen( int start, int end, int num, IntArray& datas )
{
    if( useEo != 0 )
    {
        cout << "使用eo生成随机数" << endl;
        eo_rng_gen( start, end, num, datas );
    }
    else
    {
        cout << "使用crt生成随机数" << endl;
        sys_rng_gen( start, end, num, datas );
    }
}

void rng_seed( unsigned long seed )
{
    if( useEo != 0 )
    {
        eo_rng_seed( seed );
    }
    else
    {
        sys_rng_seed( seed );
    }
}

// 打印或写入数据到输出流(控制台输出或文件输出)
void print_datas( ostream& os, IntArray& datas )
{
    int n = datas.size();
    for( int i = 0; i < n; i++ )
    {
        os << datas[i];
        if( i < n - 1 ) os << "\t";
    }
    os << endl;
}

// 分析随机数出现的次数
void analye_datas( IntArray& datas, int c[] )
{
    int n = datas.size();
    for( int i = 0; i < n; i++ )
    {
        c[datas[i] - DATA_START - 1]++;
    }
}

void rng_total_test( const char* filename, int runCount, bool fixSeed = false )
{
    cout << "\n输出结果保存在" << filename << "中" << endl;

    // 记录随机数出现的次数
    int c[DATA_NUM] = {0};

    int seed = SEED;
    if( !fixSeed ) seed = time( NULL );

    // 设置种子
    rng_seed( seed );
    ofstream outfile( filename );

    outfile << "区间[" << DATA_START + 1 << "," << DATA_END << "]"
            << ", 总运行次数:" << runCount << "次"
            << ", 每一次生成的随机数个数:" << DATA_NUM << "个"
            << ", 种子:" << seed
            << endl;

    // 生成随机数
    IntArray datas;
    rng_gen( DATA_START, DATA_END, DATA_NUM * runCount, datas );

    // 写入随机数到文件
    print_datas( outfile, datas );

    // 统计随机数出现次数
    analye_datas( datas, c );

    outfile << "\n统计随机数出现次数:"
            << "\n数字\t次数\n";
    // 输出统计次数
    for( int i = 0; i < DATA_NUM; i++ )
    {
        outfile << i + DATA_START + 1 << "\t" << c[i] << endl;
    }
    outfile << endl;

    outfile.close();
}

int main()
{
    cout << "1) 固定种子测试.." << endl;
    rng_total_test( "test0.txt", 10, true );;

    cout << "\n2) 随机测试,以当前时间作为种子.." << endl;
    cout << "\n使用c语言自带的随机函数?(0表示使用，1表示不使用):";
    cin >> useEo;

    cout << "\n请输入测试次数(每1次生成100个数据):";
    int runCount;
    cin >> runCount;

    // 生成随机数，并写入到文件test.txt
    rng_total_test( "test.txt", runCount );

    return 0;
}
