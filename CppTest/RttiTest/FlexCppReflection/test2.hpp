/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

// tests abstract types

#include "TestHelpers.h"

namespace Flex_Test_
{

    class IA
    {
    public:
        //IA() { PRINT_FN; }
        virtual ~IA()
        {
            PRINT_FN;
        }

        virtual void pure() = 0;
    };

    class IB
    {
    public:
        //IB() { PRINT_FN; }
        virtual ~IB()
        {
            PRINT_FN;
        }

        virtual void virt() = 0;
    };

    class D : public IA, public IB
    {
    public:
        D()
        {
            PRINT_FN;
        }
        virtual ~D()
        {
            PRINT_FN;
        }

        virtual void pure() {}
        virtual void virt() {}
    };

    void test2();

}

FLEX_DECLARE_ABSTRACT_TYPE(Flex_Test_::IA, 0, ())
FLEX_DECLARE_ABSTRACT_TYPE(Flex_Test_::IB, 0, ())
FLEX_DECLARE_TYPE(Flex_Test_::D, 2, (Flex_Test_::IA, Flex_Test_::IB))
