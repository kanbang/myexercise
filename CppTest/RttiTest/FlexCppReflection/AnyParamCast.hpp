#pragma once

#include <boost/type_traits.hpp>

#include "AnyCast.hpp"

namespace Flex_
{

    namespace Detail_
    {
        template <typename T, bool IsPtr = true> struct RefIfNotPtr
        {
            typedef T type;
        };

        template <typename T> struct RefIfNotPtr<T, true>
        {
            typedef typename boost::add_reference<T>::type type;
        };

        template <typename T> struct MakeRefOrPtr
        {
            typedef typename RefIfNotPtr<T, boost::is_pointer<T>::value>::type type;
        };

        template <typename T, bool IsPtr = true>
        struct FancyAnyCast
        {
            typedef T type;
            static T cast(AnyPtr const& ptr)
            {
                return AnyCast< T >(ptr);
            }
        };

        template <typename T>
        struct FancyAnyCast<T, false>
        {
            typedef typename boost::add_reference<T>::type type;
            static type cast(AnyPtr const& ptr)
            {
                return *(AnyCast< T >(ptr));
            }
        };
    }

    template <typename T>
    struct ParamCastResult   // ....wait.. isn't this just MakeRefOrPtr?
    {
        typedef typename Detail_::FancyAnyCast<T, boost::is_pointer<T>::value>::type type;
    };

    template <typename T>
    typename ParamCastResult<T>::type AnyParamCast( AnyPtr const& ptr )
    {
        return Detail_::FancyAnyCast<T, boost::is_pointer<T>::value>::cast( ptr );
    }

}
