/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

// mem var tests

#include "TestHelpers.h"
#include "MemberVariables.hpp"

namespace Flex_Test_
{

    class V
    {
        int x;
        float y;
        float y2;
        float* yp;

        int foo();
        int bar(float);

        friend struct Flex_::MemberVariableList< V>;
    public:
        V() : x(10), y(-1.618f), y2(.618f), yp(&y)
        {	}
        ~ V() {}
    };

    void test5();

}

FLEX_DECLARE_ABSTRACT_TYPE(Flex_Test_::V, 0, ())
FLEX_DECLARE_VARIABLES(Flex_Test_::V, 4, (x, y, y2, yp))
