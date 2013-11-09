#include "hellloworld.h"
#include <QtGui/QApplication>

int main( int argc, char* argv[] )
{
    QApplication a( argc, argv );
    HellloWorld w;
    w.show();
    return a.exec();
}
