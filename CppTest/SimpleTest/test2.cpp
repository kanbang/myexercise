#include <iostream>

using namespace std;

int main()
{
    int a = 10;
    long ptr = ( long )&a;

    double b = 1.22;
    double* p = &b;
    long ptr2 = ( long )p;
    void* ptr3 = &b;
    cout << &a << endl;
    cout << ptr << endl;
    cout << ptr2 << endl;
    cout << *( ( int* )ptr ) << endl;
    cout << *( ( double* )ptr2 ) << endl;
    cout << *( ( double* )ptr3 ) << endl;

    double& bb = b;
    cout << &b << endl;
    cout << &bb << endl;
    return 0;
}
