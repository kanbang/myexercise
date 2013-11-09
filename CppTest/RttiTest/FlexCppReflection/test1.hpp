/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

// tests general/basic functionality
// creating some types in a small hierarchy

#include "TestHelpers.h"

namespace Flex_Test_
{

    class A
    {
    public:
        A()
        {
            PRINT_FN;
        }
        virtual ~A()
        {
            PRINT_FN;
        }
    };

    class B : public A
    {
    public:
        B()
        {
            PRINT_FN;
        }
        virtual ~B()
        {
            PRINT_FN;
        }

    private:
        char data[70];
    };

    class C : public B
    {
    public:
        C()
        {
            PRINT_FN;
        }
        virtual ~C()
        {
            PRINT_FN;
        }
    };

    void test1();

}

FLEX_DECLARE_TYPE(Flex_Test_::A, 0, ())
FLEX_DECLARE_TYPE(Flex_Test_::C, 1, (Flex_Test_::B))
FLEX_DECLARE_TYPE(Flex_Test_::B, 1, (Flex_Test_::A))


