#pragma once

#include <string>
#include <utility>

#include <boost/unordered_map.hpp>

#include "MemberFunction.hpp"

namespace Flex_
{

    class MemberFunctions
    {
        typedef boost::unordered_map<std::string, MemberFunction> FunctionMap;

    public:

        MemberFunctions& AddFunction(std::string const& name, MemberFunction const& fn);

        MemberFunction const& GetFunction(std::string const& name) const;
        bool HasFunction(std::string const& name) const;

        typedef FunctionMap::const_iterator FunctionIter;
        typedef std::pair<FunctionIter, FunctionIter> FunctionRange;
        FunctionRange GetAllFunctions() const;

        bool Empty() const;

    private:

        FunctionMap functions_;
    };

}

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/array.hpp>
#include <boost/preprocessor/array/elem.hpp>

#define FLEX_MAKE_OBJ_FNS_IMPL(Z, I, CLASS_FNS)\
	.AddFunction(BOOST_PP_STRINGIZE(BOOST_PP_ARRAY_ELEM(I,BOOST_PP_TUPLE_ELEM(2,1,CLASS_FNS))),\
	::Flex_::makeAnyFunction( &BOOST_PP_TUPLE_ELEM(2,0,CLASS_FNS)::BOOST_PP_ARRAY_ELEM(I,BOOST_PP_TUPLE_ELEM(2,1,CLASS_FNS))) )
#define FLEX_MAKE_OBJ_FNS(CLASS_NAME, NUM_FNS, FN_TUPLE)\
	Flex_::MemberFunctions() BOOST_PP_REPEAT(NUM_FNS, FLEX_MAKE_OBJ_FNS_IMPL, (CLASS_NAME,(NUM_FNS,FN_TUPLE)))

