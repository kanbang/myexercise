#include <iostream>
#include <iomanip>
using namespace std;

void main()
{
    cout << "��һ��" << endl;
    cout << "    ";
    cout << setiosflags( ios::left ) << setw( 7 ); //���ÿ��Ϊ7��left���뷽ʽ
    cout << "1.1";
    cout << "ʲô��C����";
    cout << resetiosflags( ios::left );            //ȡ�����뷽ʽ
    cout << setfill( '.' ) << setw( 30 ) << 1 << endl; //���Ϊ30�����Ϊ'.'���
    cout << setfill( ' ' );                        //�ָ����Ϊ�ո�
    cout << "    ";
    cout << setw( 7 ) << setiosflags( ios::left ); //���ÿ��Ϊ7��left���뷽ʽ
    cout << "1.11";
    cout << "C���Ե���ʷ";
    cout << resetiosflags( ios::left );            //ȡ�����뷽ʽ
    cout << setfill( '.' ) << setw( 30 ) << 58 << endl; //���Ϊ30�����Ϊ'.'���
    cout << setfill( ' ' ) << "�ڶ���" << endl;
}

