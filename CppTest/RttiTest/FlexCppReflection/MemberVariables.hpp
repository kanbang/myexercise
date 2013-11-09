/***************************************************************************
*   Copyright (C) 2008 by Tyler Shaub   *
*   tyler.shaub@gmail.com   *
*                                                                         *
*   See COPYING	*
***************************************************************************/
#pragma once

#include <map>
#include <string>

#include <boost/mpl/list.hpp>

#include "AnyPtr.hpp"
#include "AnyCast.hpp"
#include "MemberVariable.hpp"

namespace Flex_
{

    class MemberVariables
    {
    public:
        MemberVariables();

        // default cpy, asn, dtor

        void SetVariables(MemberVariablePtrMap const& varPtrs);

        AnyPtr GetMemberVariable(std::string const& memberName, AnyPtr const& object) const;

        typedef MemberVariablePtrMap::const_iterator MemberVariablePtrIter;
        typedef std::pair<MemberVariablePtrIter,MemberVariablePtrIter> MemberVariablePtrRange;

        MemberVariablePtrRange GetAllMemberVariablePtrs() const;

        bool Empty() const;

        // include a for-each that uses static info (typelist)


    private:

        MemberVariablePtrMap variables_;
    };

    template <typename Class_, typename Variable_>
    class MemberVariableWrapper : public MemberVariable
    {
    private:
        typedef Variable_ Class_::* MemVarPtr;

    public:
        typedef Class_ Class;
        typedef Variable_ Variable;

        MemberVariableWrapper(Variable Class::* ptr) : memVarPtr_(ptr) {}

        virtual AnyPtr GetPointer(AnyPtr const& object) const
        {
            using Flex_::AnyCast;
            if(object.Mutable())
                return getPointer<Class>(AnyCast<Class>(object));
            else
                return getPointer<const Class>(AnyCast<const Class>(object));
        }

        AnyPtr GetPointer(Class* object) const
        {
            return getPointer<Class>(object);
        }

        AnyPtr GetPointer(Class const* object) const
        {
            return getPointer<const Class>(object);
        }

    private:

        template <typename ClassWithQualifiers>
        AnyPtr getPointer(ClassWithQualifiers* object) const
        {
            return AnyPtr( &(object->*memVarPtr_) );
        }

        MemVarPtr memVarPtr_;
    };

    template <typename Class>
    struct MakeAnyMemberVariable
    {

        template <typename Variable>
        static MemberVariableWrapper<Class,Variable> Make(Variable Class::* ptr)
        {
            return MemberVariableWrapper<Class,Variable>(ptr);
        }
    };

}

#include "TypeOf.hpp"

#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum.hpp>

#define MEMBER_TYPE(T, V) FLEX_TYPEOF( T().V )
#define MEMBER_POINTER_TYPE(T, V) MEMBER_TYPE( T, V ) T::*

#define MAKE_TYPE_LIST_IMPL(z, I, V) MEMBER_TYPE( Type, BOOST_PP_ARRAY_ELEM(I,V) )
#define MAKE_TYPE_LIST(N, V) BOOST_PP_ENUM(N, MAKE_TYPE_LIST_IMPL, (N, V) )

// FIXME: code bloat. consider new'ing these guys
#define MAKE_VARIABLE_PTRS_IMPL(z, I, V)\
	static const MemberVariableWrapper<Type, MEMBER_TYPE(Type, BOOST_PP_ARRAY_ELEM(I,V))> \
	BOOST_PP_ARRAY_ELEM(I,V) = MakeAnyMemberVariable<Type>::Make(&Type::BOOST_PP_ARRAY_ELEM(I,V));
#define MAKE_VARIABLE_PTRS(N, V) BOOST_PP_REPEAT(N, MAKE_VARIABLE_PTRS_IMPL, (N,V) )

#define MAKE_VARIABLE_PTR_LIST_IMPL(z, I, V)\
	variables.insert( std::make_pair(std::string(BOOST_PP_STRINGIZE(BOOST_PP_ARRAY_ELEM(I,V))), &BOOST_PP_ARRAY_ELEM(I,V)) );
#define MAKE_VARIABLE_PTR_LIST(N, V) BOOST_PP_REPEAT(N, MAKE_VARIABLE_PTR_LIST_IMPL, (N,V) )


#define FLEX_DECLARE_VARIABLES(T, NumVars, VarTuple)\
	namespace Flex_ { template <> struct MemberVariableList<T> {\
		typedef T Type;\
		typedef boost::mpl::list< MAKE_TYPE_LIST(NumVars, VarTuple) > VariableTypeList;\
		\
		typedef std::map<std::string, Flex_::MemberVariable const*> VariablePtrs;\
		static VariablePtrs GetVariablePtrs() {\
			MAKE_VARIABLE_PTRS(NumVars, VarTuple)\
			VariablePtrs variables;\
			MAKE_VARIABLE_PTR_LIST(NumVars, VarTuple); \
			return variables;\
		}\
	}; }
