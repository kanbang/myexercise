// ���Կ������캯���͸�ֵ�����
#include <iostream>

using namespace std;

class A
{
public:
    A( int i ): m_i( i )
    {
        cout << "���ù��캯��" << endl;
    }
    A( const A& a )
    {
        cout << "���ÿ������캯��" << endl;
        m_i = a.m_i;
    }

    A& operator=( const A& a )
    {
        cout << "���ø�ֵ���������" << endl;
        this->m_i = a.m_i;
        return *this;
    }
private:
    int m_i;
};

void f1( const A& a )
{
    cout << "���ú���f1()��ʹ��const����" << endl;
}

void f2( A a )
{
    cout << "���ú���f2(),ʹ�ø���" << endl;
}

void f3( A& a )
{
    cout << "���ú���f3(),ʹ������" << endl;
}

A f4()
{
    cout << "���ú���f4()��ֱ�ӷ�������" << endl;
    return 1;
}

A f5()
{
    cout << "���ú���f5()��ֱ�ӷ��ؾֲ�����" << endl;
    A a( 11 );
    return a;
}

A f6()
{
    cout << "���ú���f6()��ֱ�ӷ�����ʱ����" << endl;
    return A( 11 );
}

int main()
{
    cout << "-----------------------" << endl;
    A a( 10 );
    cout << "-----------------------" << endl;
    A b = a;
    cout << "-----------------------" << endl;
    A c( a );
    cout << "-----------------------" << endl;
    c = b;
    cout << "-----------------------" << endl;

    cout << endl;
    cout << "************************" << endl;
    A d = 200;
    cout << "************************" << endl;
    A e( 300 );
    cout << "************************" << endl;
    e = 1000;
    cout << "************************" << endl;

    cout << endl;
    cout << "-----------------------" << endl;
    f1( c );
    cout << "-----------------------" << endl;
    f2( c );
    cout << "-----------------------" << endl;
    f1( 120 );
    cout << "-----------------------" << endl;
    f2( 100 );
    cout << "-----------------------" << endl;
    //f3(1000); // ȡ��ע�ͣ��õ�C2664 error

    A f = f4();
    cout << "-----------------------" << endl;
    A g = f5();
    cout << "-----------------------" << endl;
    A h = f6();
    cout << "-----------------------" << endl;
    return 0;
}