#pragma once

#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin_end.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_pointer.hpp>


// not-as-awesome version of boost::mpl::for_each runtime algo
// does NOT construct argument of type, instead passes type as tpl param
// this means we require an object F with a known nullary function 'execute' taking the tpl param

namespace Flex_
{

    namespace Detail_
    {
        template< bool done = true >
        struct for_each_type_impl
        {
            template<
            typename Iterator
            , typename LastIterator
            , typename F
            >
            static void execute(Iterator*, LastIterator*, F )
            {
            }
        };

        template<>
        struct for_each_type_impl<false>
        {
            template<
            typename Iterator
            , typename LastIterator
            , typename F
            >
            static void execute(Iterator*, LastIterator*, F f)
            {
                typedef typename boost::mpl::deref<Iterator>::type item;
                typedef typename boost::mpl::next<Iterator>::type iter;

                f.execute<item>( );

                for_each_type_impl<boost::is_same<iter,LastIterator>::value>::
                execute((iter*)0, (LastIterator*)0,  f);
            }
        };
    }

    template<typename Sequence, typename F>
    inline void for_each_type(F f)
    {
        typedef typename boost::mpl::begin<Sequence>::type first;
        typedef typename boost::mpl::end<Sequence>::type last;

        Detail_::for_each_type_impl< boost::is_same<first,last>::value >::
        execute((first*)0, (last*)0, f);
    }


}
