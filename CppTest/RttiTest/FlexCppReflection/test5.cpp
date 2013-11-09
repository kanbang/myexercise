/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#include "test5.hpp"

#include <iostream>

#include "TypeManagerHelpers.hpp"
#include "AnyPtr.hpp"

FLEX_DECLARE_TYPE( float, 0, () )

namespace Flex_Test_
{

    template <typename V, typename T>
    V& GetMemVar( char const* name, T& t )
    {
        return *Flex_::AnyCast<V>( ::Flex_::GetInfoFromType<T>().GetVariables().GetMemberVariable( name, &t ) );
    }

#define gv(n, p) Flex_::GetInfoFromType<V>().GetVariables().GetMemberVariable(n, p)


    void test5()
    {

        V c;
        int types = FLEX_TYPES.TotalTypesCreated();

        GetMemVar<int>( "x", c ) = -7; // c.x = -7

        //gv("x", &c)
        Flex_::AnyPtr x = Flex_::GetInfoFromType<V>().GetVariables().GetMemberVariable( "x", &c );
        int xi = *Flex_::AnyCast<const int>( x ); // must use 'const int' since c is const => xp is "int const*"

        std::cout << "c::x = " << xi << std::endl;

        Flex_::AnyPtr yp = Flex_::GetInfoFromType<V>().GetVariables().GetMemberVariable( "yp", &c );
        float** yfp = Flex_::AnyCast<float**>( yp );

        Flex_::AnyPtr y2 = Flex_::GetInfoFromType<V>().GetVariables().GetMemberVariable( "y2", &c );
        *yfp = Flex_::AnyCast<float*>( y2 );
    }

}
