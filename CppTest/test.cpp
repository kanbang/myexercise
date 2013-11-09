#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

namespace RS2
{
    enum SnapMode
    {
        SnapFree = 0,         /**< Free positioning */
        SnapEndpoint = 1,     /**< Snap to endpoints */
        SnapMiddle = 2,       /**< Snap to middle points */
        SnapCenter = 4,       /**< Snap to centers */
        SnapOnEntity = 8,     /**< Snap to the next point on an entity */
        SnapDist = 16,         /**< Snap to points with a distance to an endpoint */
        SnapIntersection = 32, /**< Snap to intersection */
        SnapIntersectionManual = 64  /**< Snap to intersection manually */
    };
}

enum EntityFlags
{
    EF_Visible     = 1 << 0,
    EF_Selected    = 1 << 1,
    EF_Highlighted = 1 << 2,
    EF_Erased      = 1 << 3
};


int main()
{
    cout << 0x123456 << endl;

    double power = 4800;
    double theta = 0.02;
    double rho = 1.2;
    double c_p = 1.0006;
    double delta_t = 2;
    double q = 3600 * power * theta / ( rho * c_p * 60 * delta_t );

    cout << q << endl;

    int defaultSnapMode = RS2::SnapEndpoint | RS2::SnapMiddle | RS2::SnapCenter;

    cout << defaultSnapMode << endl;
    cout << ( defaultSnapMode & RS2::SnapEndpoint ) << endl;
    cout << ( defaultSnapMode & RS2::SnapEndpoint != 0 ) << endl;
    cout << ( defaultSnapMode & RS2::SnapDist ) << endl;
    cout << ( defaultSnapMode & RS2::SnapDist != 0 ) << endl;

    cout << ( 1 << 0 ) << endl;
    cout << ( 1 << 1 ) << endl;


    cout << "--------------------------" << endl;

    int flags = EF_Visible | EF_Selected;
    cout << flags << endl;
    cout << ( flags ^ EF_Selected ) << endl;
    cout << ( flags & ( ~EF_Selected ) ) << endl;
    cout << ( ~EF_Selected ) << endl;
    cout << ( flags & EF_Erased ) << endl;

    int a = 3, b = 4;
    int* pa = &a, *pb = &b;
    cout << pa << endl;
    cout << pb << endl;
    cout << ( pa > pb ) << endl;

    int key = '1';
    cout << key << endl;

    char str[4];
    sprintf( str, "%.2f", 1.234 );
    cout << str << "  " << strlen( str ) << endl;

    sprintf( str, "%d", 50 );
    cout << str << "  " << strlen( str ) << endl;
    return 0;
}
