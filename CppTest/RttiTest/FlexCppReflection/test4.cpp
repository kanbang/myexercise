/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#include "test4.hpp"
#include "TestHelpers.h"

namespace Flex_Test_
{

    struct IntAllocator
    {
        static void* create()
        {
            PRINT_FN;
            return new int( 1337 );
        }

        static void* clone( void const* copy )
        {
            PRINT_FN;
            return new int( * static_cast<int const*>( copy ) );
        }

        static void destruct( void* o )
        {
            PRINT_FN;
            *static_cast<int*>( o ) = 0;
        }

        static void destroy( void* o )
        {
            PRINT_FN;
            delete static_cast<int*>( o );
        }
    };

    void test4()
    {
        using std::cout;
        using std::endl;
        using namespace Flex_;

        cout << endl << "-----------------------------------------" << endl << endl;
        {
            // manual TypeInfo creation of a primitive type - fancy
            TypeManager& f = TypeManager::GetInst();
            TypeInfo intInfo = f.CreateTypeInfo();

            intInfo.SetSize( sizeof( int ) );
            intInfo.SetName( "int" );

            intInfo.SetReferenceType( intInfo.GetId() );

            Flex_::SetCreationFunctions<IntAllocator>( intInfo ); // for fun: std ctor is too boring

            // short hand for above is just: FLEX_TYPE_INFO_ALLOC(int, IntAllocator)

            f.RegisterType<int>( intInfo );

            int* i = Create<int>( "int" );

            // does '*i' implement static type 'int' ?
            cout << "Make extra sure we have a correct interface: "
                 << std::boolalpha << ImplementsStatic<int>( *i ) << endl;

            GetInfoFromObj( *i ).Print();
            cout << "new int value: " << *i << endl;

            Destroy( i );

            // also note, creation method does not dictate what info we can derive
            int* i2 = new int;
            *i2 = 666;

            cout << "Typeid from rand obj (new int): " <<
                 f.GetIdFromStdInfo( typeid( *i2 ) ) << endl;

            delete i2;
        }
    }

}
