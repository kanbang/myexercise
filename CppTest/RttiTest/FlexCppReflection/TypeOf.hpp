#pragma once

#include <typeinfo>

namespace Flex_
{

// super-simple, straight forward, (not standard-conformant!) typeof
// BOOST_TYPEOF strongly recommended
    template < std::type_info const* >
    struct TypeOf
    {
        /*typedef T type;*/
    };

}

#ifdef FLEX_NO_BOOST_TYPEOF
#	define FLEX_REGISTER_TYPEOF(T)\
	namespace Flex_  { template <> struct TypeOf< &(typeid(T)) > { typedef T type; }; }
#	define FLEX_TYPEOF(t) Flex_::TypeOf< & (typeid(t)) >::type
#	define FLEX_TYPEOF_TPL typename FLEX_TYPEOF
#else
#	include <boost/typeof/typeof.hpp>
#	define FLEX_REGISTER_TYPEOF BOOST_TYPEOF_REGISTER_TYPE
#	define FLEX_TYPEOF BOOST_TYPEOF
#	define FLEX_TYPEOF_TPL BOOST_TYPEOF_TPL
#endif
