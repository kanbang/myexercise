#pragma once

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include <boost/call_traits.hpp>

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/back_inserter.hpp>

#include <boost/type_traits.hpp>
#include <boost/utility.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include "ForEachType.hpp"

namespace Flex_
{

    class AnyPtr;
    class AnyParams;

    namespace bmp = boost::mpl;
    namespace bft = boost::function_types;

    namespace Detail_
    {

        template <typename T>
        struct IsValueType   // not pointer or ref
        {
            static const bool value = !(boost::is_reference<T>::value || boost::is_pointer<T>::value);
        };

        template <typename T> struct GetPointer
        {
            static typename boost::remove_reference<T>::type*
            Get( typename boost::call_traits<T>::param_type obj)
            {
                return boost::addressof(obj);
            }
        };
        template <typename T> struct GetPointer<T*>
        {
            static T* Get( T* obj)
            {
                return obj;
            }
        };

        template <typename R, bool ReturnIsValue = false> struct AssignReturnVal
        {
            static void Assign( R result, Flex_::AnyPtr& retval)
            {
                retval = GetPointer<R>::Get(result); // store result as pointer
            }
        };
        template <typename R> struct AssignReturnVal<R,true>
        {
            static void Assign( typename boost::call_traits<R>::param_type result, Flex_::AnyPtr& retval)
            {
                if(!retval.Empty()) // copy in result value
                    (*Flex_::AnyCast<R>(retval)) = result;
            }
        };

#define FLEX_LIST_PARAMS_IMPL(z, I, NA) params.retrieveParam< typename bmp::at<ParamTypes, bmp::size_t<I> >::type >(I)
#define FLEX_LIST_PARAMS(N) BOOST_PP_ENUM(N, FLEX_LIST_PARAMS_IMPL, NA )

#define FLEX_GEN_INVOKE_FNS(z, SIZE, NA)\
	template <typename Func, typename ReturnType, typename ParamTypes, typename ThisType>					\
	struct Invoke<Func, ReturnType, ParamTypes, ThisType, false, SIZE> {									\
		static void invoke(Func const& fnPtr, ThisType * obj, AnyParams const& params, Flex_::AnyPtr& returnOut)		\
		{																									\
			AssignReturnVal<ReturnType, IsValueType<ReturnType>::value >::Assign(				\
			(obj->*fnPtr)( FLEX_LIST_PARAMS(SIZE) ), returnOut);										\
		}																									\
	};																										\
	template <typename Func, typename ReturnType, typename ParamTypes, typename ThisType>					\
	struct Invoke<Func, ReturnType, ParamTypes, ThisType, true, SIZE> {										\
		static void invoke(Func const& fnPtr, ThisType * obj, AnyParams const& params, Flex_::AnyPtr&)		\
		{																									\
			(obj->*fnPtr)( FLEX_LIST_PARAMS(SIZE) );																\
		}																									\
	};

        template <typename Func, typename ReturnType, typename ParamTypes, typename ThisType, bool ReturnsVoid, size_t FunctionArity>
        struct Invoke
        {
            static void invoke(Func const& , ThisType* , AnyParams const&, Flex_::AnyPtr& )
            {
                assert(false); // function arity not supported (add specialization(s))
            }
        };

        BOOST_PP_REPEAT(10, FLEX_GEN_INVOKE_FNS, NA)
    }

    template <typename TypeManager>
    struct FunctionParamsVector
    {
    private:
        typedef typename bft::parameter_types< TypeManager >::type ParamSeq;
        typedef typename bmp::front<ParamSeq>::type ThisType;
        typedef typename bmp::pop_front<ParamSeq>::type NoThisParamSeq;

    public:

        typedef typename bmp::copy<NoThisParamSeq, bmp::back_inserter< bmp::vector0<> > >::type param_types;
        typedef typename boost::remove_reference< ThisType >::type this_type;
    };

    template <typename F>
    struct MemFunctionWrapper
    {
        typedef F FunctionType;
        static const typename bft::function_arity<F>::value_type Arity
            = bft::function_arity<F>::value-1; // subtracting 1 for 'this' ptr
        typedef typename bft::result_type< F >::type ReturnType;
        typedef FunctionParamsVector<F> FunctionParams;
        typedef typename FunctionParams::param_types ParamTypes;
        typedef typename FunctionParams::this_type ThisType;

        MemFunctionWrapper(F const& f) : functionPtr(f) {}

        typedef std::vector<TypeId> ParamTypeIds;
        ParamTypeIds GetParamTypeIds() const
        {
            ParamTypeIds ids;

            for_each_type< ParamTypes >( ParamIdAccum(ids) );

            return ids;
        }

        void operator() (AnyPtr const& thisPtr, AnyParams const& params, AnyPtr& returnOut) const
        {
            assert( params.size() == Arity );

            ThisType* obj = AnyCast<ThisType>(thisPtr);

            Detail_::Invoke<FunctionType, ReturnType, ParamTypes, ThisType,
                    boost::is_void<ReturnType>::value, Arity>::invoke(functionPtr, obj, params, returnOut);
        }

    private:

        struct ParamIdAccum
        {
            ParamTypeIds& params_;
            ParamIdAccum(ParamTypeIds& params) : params_(params) {}
            template <typename ParamT> void execute ()
            {
                params_.push_back( TypeManager::GetInst().GetIdFromType<ParamT>() );
            }
        };

        FunctionType functionPtr;
    };

}

