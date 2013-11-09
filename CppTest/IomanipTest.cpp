#include <iostream>
#include <iomanip>
using namespace std;

void main()
{
    cout << "第一章" << endl;
    cout << "    ";
    cout << setiosflags( ios::left ) << setw( 7 ); //设置宽度为7，left对齐方式
    cout << "1.1";
    cout << "什么是C语言";
    cout << resetiosflags( ios::left );            //取消对齐方式
    cout << setfill( '.' ) << setw( 30 ) << 1 << endl; //宽度为30，填充为'.'输出
    cout << setfill( ' ' );                        //恢复填充为空格
    cout << "    ";
    cout << setw( 7 ) << setiosflags( ios::left ); //设置宽度为7，left对齐方式
    cout << "1.11";
    cout << "C语言的历史";
    cout << resetiosflags( ios::left );            //取消对齐方式
    cout << setfill( '.' ) << setw( 30 ) << 58 << endl; //宽度为30，填充为'.'输出
    cout << setfill( ' ' ) << "第二章" << endl;
}

