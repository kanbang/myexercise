/***************************************************************************
*   Copyright (C) 2008 by Tyler Shaub   *
*   tyler.shaub@gmail.com   *
*                                                                         *
*   See COPYING	*
***************************************************************************/

#pragma once

#include "ForEachType.hpp"

#include "TypeManager.hpp"
#include "TypeInfoHelpers.hpp"
#include "MemberVariable.hpp"
#include "TypeOf.hpp"

namespace Flex_
{

    template <typename T>
    struct StaticTypeInfo
    {
        static TypeId GetTypeId();
    };

    template <typename T>
    struct AutoRegister
    {
        AutoRegister()
        {
            StaticTypeInfo<T>::GetTypeId();
        }
        inline void nop() const {}
    };

    template <typename T>
    struct AutoRegistered
    {
    protected:
        static const AutoRegister<T> createReg_;
    };
    template <typename T> const AutoRegister<T> AutoRegistered<T>::createReg_;

    template <typename D>
    struct AddBase
    {
        TypeInfo* info;
        AddBase(TypeInfo& info) : info(&info) {}

        template <typename Base>
        void execute ()
        {
            namespace bmpl = boost::mpl;
            typedef StaticTypeInfo<Base> StaticBaseInfo;

            TypeManager& f = TypeManager::GetInst();

            TypeInfo const& baseInfo =  f.GetInfoFromId( StaticBaseInfo::GetTypeId() );

            info->AddInterface( baseInfo );
            f.AddTypeImplementation( baseInfo.GetId(), info->GetId() );

            // "from" info	 to type id	,	anyStaticCast<ToType, FromType>
            info->AddCast( baseInfo.GetId(), &AnyStaticCast<Base, D> );

            baseInfo.AddCast( info->GetId(), &AnyStaticCast<D, Base> );

            for_each_type< typename StaticBaseInfo::BaseTypes >( AddBase<D>(*info) );
        }
    };

    template <typename T, typename Alloc, typename BaseTypes>
    TypeInfo CreateTypeInfo(char const* typeName)
    {
        TypeInfo newInfo = TypeManager::GetInst().CreateTypeInfo();

        newInfo.SetSize(sizeof(T));
        newInfo.SetName(typeName);

        newInfo.SetReferenceType( !boost::is_pointer<T>::value ?
                                  newInfo.GetId()	:
                                  StaticTypeInfo< typename boost::remove_pointer<
                                  typename boost::remove_pointer<T>::type>::type >::GetTypeId() );

        for_each_type< BaseTypes >( AddBase<T> (newInfo) );

        newInfo.SetCreationFunctions(&Alloc::create, &Alloc::clone, &Alloc::destruct, &Alloc::destroy );

        newInfo.SetVariables( MemberVariableList<T>::GetVariablePtrs() );

        return newInfo;
    }

}

#include <boost/mpl/list.hpp>
#include <boost/preprocessor/tuple/rem.hpp>

// places definition in Flex_ namspace
// MUST BE USED AT GLOBAL SCOPE! (outside any other namespaces)
#define FLEX_WRITE_STATIC_TYPE_INFO(Type, Name, Alloc, NumBases, BaseTuple)\
namespace Flex_ {\
template <> struct StaticTypeInfo<Type> : public AutoRegistered<Type>\
{\
	typedef boost::mpl::list< BOOST_PP_TUPLE_REM_CTOR(NumBases, BaseTuple) > BaseTypes;\
	static TypeId GetTypeId()\
	{\
		createReg_.nop();\
		static TypeId id = Flex_::TypeManager::GetInst().RegisterType<Type>(\
			Flex_::CreateTypeInfo< Type, Alloc, BaseTypes >(Name));\
		return id;\
	}\
};\
}

#define FLEX_FULL_DECLARE_TYPE(_T, Name, Alloc, NumBases, BaseTuple)\
		FLEX_REGISTER_TYPEOF(_T)\
		FLEX_WRITE_STATIC_TYPE_INFO(_T, Name, Alloc, NumBases, BaseTuple)

#define FLEX_DECLARE_TYPE_CUSTOM_ALLOC(T, Alloc, NumBases, BaseTuple)\
	FLEX_FULL_DECLARE_TYPE(T, #T, Alloc, NumBases, BaseTuple )

#define FLEX_DECLARE_TYPE(T, NumBases, BaseTuple)\
	FLEX_DECLARE_TYPE_CUSTOM_ALLOC(T, Flex_::StdTypeFns<T>, NumBases, BaseTuple)\
	FLEX_DECLARE_TYPE_CUSTOM_ALLOC(T*, Flex_::StdTypeFns<T*>, 0, ())\
	FLEX_DECLARE_TYPE_CUSTOM_ALLOC(T**, Flex_::StdTypeFns<T**>, 0, ())

#define FLEX_DECLARE_ABSTRACT_TYPE(T, NumBases, BaseTuple)\
	FLEX_DECLARE_TYPE_CUSTOM_ALLOC(T, Flex_::AbstractTypeFns, NumBases, BaseTuple)\
	FLEX_DECLARE_TYPE_CUSTOM_ALLOC(T*, Flex_::StdTypeFns<T*>, 0, ())\
	FLEX_DECLARE_TYPE_CUSTOM_ALLOC(T**, Flex_::StdTypeFns<T**>, 0, ())

