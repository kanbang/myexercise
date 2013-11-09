#include <iostream>
#include "geo.h"

using namespace std;

int main()
{
    RS_Point2D p( 8.0, 13 ), q;
    RS_Vector2D v;
    v = p - RS_ORIGIN;
    q = RS_ORIGIN + v;

    RS_Point2D t = p + RS_Vector2D( 10, 20 );

    RS_Point2D z( 0, 0 );

    if( p == q )
    {
        cout << "equal point" << endl;
    }

    if( z == RS_ORIGIN )
    {
        cout << "equal orign " << endl;
    }

    cout << "len=" << v.squared_length() << endl;
    cout << p.x() << "  " << p.y() << endl;

    RS_Vector2D v1 = v * 2;
    RS_Vector2D v2 = 2 * v;
    cout << "v1.x-" << v1.x() << " v1.y=" << v1.y() << endl;
    cout << "v2.x-" << v2.x() << " v2.y=" << v2.y() << endl;

    return 0;
}