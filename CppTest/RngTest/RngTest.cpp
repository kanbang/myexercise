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

/* eo�ļ��а���һ��rngȫ�ֶ���
 * extern eoRng rng;
 * */

/* ������̶����� */
const unsigned long SEED = 42;
/* ��������,����[0, 6) ==> [0, 5] */
const int DATA_START = 0;
const int DATA_END = 100;
const int DATA_NUM = DATA_END - DATA_START;
/* ������������д��� */
const int RUN_COUNT = 100;

int useEo = 0;

// ʹ��eo
void eo_rng_gen( int start, int end, int num, IntArray& datas )
{
    // ���������
    for( int i = 0; i < num; i++ )
    {
        datas.push_back( ( int )rng.uniform( start, end ) + 1 );
    }
}

void eo_rng_seed( unsigned long seed )
{
    rng.reseed( seed );
}

// ʹ��crt�Դ���rand
void sys_rng_gen( int start, int end, int num, IntArray& datas )
{
    // ���������
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
        cout << "ʹ��eo���������" << endl;
        eo_rng_gen( start, end, num, datas );
    }
    else
    {
        cout << "ʹ��crt���������" << endl;
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

// ��ӡ��д�����ݵ������(����̨������ļ����)
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

// ������������ֵĴ���
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
    cout << "\n������������" << filename << "��" << endl;

    // ��¼��������ֵĴ���
    int c[DATA_NUM] = {0};

    int seed = SEED;
    if( !fixSeed ) seed = time( NULL );

    // ��������
    rng_seed( seed );
    ofstream outfile( filename );

    outfile << "����[" << DATA_START + 1 << "," << DATA_END << "]"
            << ", �����д���:" << runCount << "��"
            << ", ÿһ�����ɵ����������:" << DATA_NUM << "��"
            << ", ����:" << seed
            << endl;

    // ���������
    IntArray datas;
    rng_gen( DATA_START, DATA_END, DATA_NUM * runCount, datas );

    // д����������ļ�
    print_datas( outfile, datas );

    // ͳ����������ִ���
    analye_datas( datas, c );

    outfile << "\nͳ����������ִ���:"
            << "\n����\t����\n";
    // ���ͳ�ƴ���
    for( int i = 0; i < DATA_NUM; i++ )
    {
        outfile << i + DATA_START + 1 << "\t" << c[i] << endl;
    }
    outfile << endl;

    outfile.close();
}

int main()
{
    cout << "1) �̶����Ӳ���.." << endl;
    rng_total_test( "test0.txt", 10, true );;

    cout << "\n2) �������,�Ե�ǰʱ����Ϊ����.." << endl;
    cout << "\nʹ��c�����Դ����������?(0��ʾʹ�ã�1��ʾ��ʹ��):";
    cin >> useEo;

    cout << "\n��������Դ���(ÿ1������100������):";
    int runCount;
    cin >> runCount;

    // �������������д�뵽�ļ�test.txt
    rng_total_test( "test.txt", runCount );

    return 0;
}
