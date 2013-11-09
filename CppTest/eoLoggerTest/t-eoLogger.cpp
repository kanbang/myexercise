#include <iostream>
using namespace std;

#include "eoLogger.h"

void f()
{
    eo::log << eo::setlevel( eo::debug );

    eo::log << eo::warnings;

    eo::log << "We are writing on the default output stream" << std::endl;

    eo::log << eo::file( "test.txt" ) << "In FILE" << std::endl;
    eo::log << std::cout << "on COUT" << std::endl;

    eo::log << eo::setlevel( "errors" );
    eo::log << eo::setlevel( eo::errors );

    eo::log << eo::quiet << "1) in quiet mode" << std::endl;

    eo::log << eo::setlevel( eo::warnings ) << eo::warnings << "2) in warnings mode" << std::endl;

    eo::log << eo::setlevel( eo::logging );

    eo::log << eo::errors;
    eo::log << "3) in errors mode";
    eo::log << std::endl;

    eo::log << eo::debug << 4 << ')'
            << "4) in debug mode\n";

}

void f2()
{
    // 初始化log
    eo::log << eo::file( "vng_log.txt" ) << "test\n";
}

void f3()
{
    eo::log << "hello world!" << endl;
    eo::log << "eo logger work " << endl;
}

int main( int ac, char** av )
{
    f2();
    f3();
    return 0;
}
