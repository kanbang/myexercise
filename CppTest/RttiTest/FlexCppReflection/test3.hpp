/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

// tests MI

#include "TestHelpers.h"


namespace Flex_Test_
{

    class B1
    {
    public:
        virtual ~B1()
        {
            PRINT_FN;
        }

        void print() const
        {
            std::cout<< "B1," << this << " :";
            PRINT_FN;
        }
        virtual void vPrint() const
        {
            std::cout<< "B1," << this << " :";
            PRINT_FN;
        }

    private:
        char data[600];
    };


    class B2
    {
    public:
        virtual ~B2()
        {
            PRINT_FN;
        }

        void print() const
        {
            std::cout<< "B2," << this << " :";
            PRINT_FN;
        }
        virtual void vPrint() const
        {
            std::cout<< "B2," << this << " :";
            PRINT_FN;
        }

    private:
        char data[200];
    };


    class D2 : public B1, public B2
    {
    public:
        virtual ~D2()
        {
            PRINT_FN;
        }

        void print() const
        {
            std::cout<< "D2," << this << " :";
            PRINT_FN;
        }
        virtual void vPrint() const
        {
            std::cout<< "D2," << this << " :";
            PRINT_FN;
        }

    private:
        char data[100];
    };

//FIXME: template <typename To, typename From>
//FIXME: To* info_cast(From * f) { return static_cast<To*>(Flex_::GetInfoFromType<To>.CastToMe<From>(f));  }

    void test3();

}

FLEX_DECLARE_TYPE(Flex_Test_::B1, 0, ())
FLEX_DECLARE_TYPE(Flex_Test_::D2, 2, (Flex_Test_::B1, Flex_Test_::B2)) // that wacky MI! (always breaking things!)
FLEX_DECLARE_TYPE(Flex_Test_::B2, 0, ())



