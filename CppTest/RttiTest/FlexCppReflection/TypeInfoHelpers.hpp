/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4100 ) // unref param does not see p->~T as using p
#endif

#include "TypeInfo.hpp"

namespace Flex_
{

    template <typename T>
    struct StdTypeFns
    {
        static void* create()
        {
            return new T();
        }
        static void destroy(void* o)
        {
            delete static_cast<T*>(o);
        }

        static void* clone(void const* c)
        {
            return c? new T( *static_cast<T const*>(c) ) : 0;
        }

        static void destruct(void* p)
        {
            static_cast<T*>(p)->~T();
        }

    };

    struct AbstractTypeFns
    {
        static void* create()
        {
            return 0;
        }

        static void* clone(void const*)
        {
            return 0;
        }

        static void destruct(void*)
        {	}

        static void destroy(void*)
        {	}
    };

    template <typename To, typename From>
    void* AnyStaticCast(void* ptr)
    {
        return static_cast<To*>( static_cast<From*>(ptr) );
    }

    template <typename T>
    void SetCreationFunctions(TypeInfo& info)
    {
        info.SetCreationFunctions(&T::create,
                                  &T::clone,
                                  &T::destruct,
                                  &T::destroy );
    }

    template <bool Abstract>
    struct SetDefaultFunctions
    {
        template <typename T>
        void operator() (TypeInfo& info)
        {
            SetCreationFunctions< StdTypeFns<T> >(info);
        }
    };

    template <>
    struct SetDefaultFunctions<true>
    {
        template <typename T>
        void operator() (TypeInfo& info)
        {
            SetCreationFunctions<AbstractTypeFns>(info);
        }
    };

}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
