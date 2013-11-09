#pragma once

#include <boost/type_traits.hpp>

namespace Flex_
{

    namespace Detail_
    {
        template <typename T>
        struct StripCvPtrRef
        {
            typedef typename boost::remove_cv
            <typename boost::remove_pointer
            <typename boost::remove_reference<T>::type>
            ::type>
            ::type type;
        };

        template <typename T>
        struct StripRefPtr
        {
            typedef typename boost::remove_pointer<
            typename boost::remove_reference<T>::type
            >::type type;
        };

        template <typename T, bool IsPtr = true>
        struct StripCvFromPtr
        {
            // strip cv from T
            // if T is pointer
            // strip pointer, strip cv, check again, add pointer
        private:
            typedef typename boost::remove_cv<T>::type NoCvT;
            typedef typename boost::remove_pointer<NoCvT>::type NoCvPtrT;
        public:
            typedef typename boost::add_pointer<
            typename StripCvFromPtr<
            NoCvPtrT, boost::is_pointer<NoCvPtrT>::value
            >::type
            >::type type;
        };

        template <typename T>
        struct StripCvFromPtr<T, /*bool IsPtr=*/false>
        {
            typedef typename boost::remove_cv<T>::type type;
        };
    }

    template <typename T>
    struct StripAllCv
    {
    private:
        typedef typename boost::remove_reference<T>::type NoRefT;
    public:
        typedef typename Detail_::StripCvFromPtr<NoRefT, boost::is_pointer<NoRefT>::value>::type type;

    };

    // explicitly trip qualifiers and ref/ptr from type
    // some compilers treat typeid qualifiers differently (incorrectly)
    template <typename T>
    struct PlainType
    {
        typedef typename Detail_::StripCvPtrRef<T>::type type;
    };

}
