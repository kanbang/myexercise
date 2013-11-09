#pragma once

#include <vector>
#include <cassert>

#include <boost/function.hpp>

#include "AnyParams.hpp"
#include "FunctionWrapperImpl.hpp"

namespace Flex_
{

    class AnyPtr;

    class MemberFunction
    {
    public:

        MemberFunction();

        template <typename F>
        MemberFunction( MemFunctionWrapper<F> const& f ) : function_( f ),
            arity_(static_cast<int>(MemFunctionWrapper<F>::Arity)), paramTypes_(f.GetParamTypeIds()),
            returnType_( Flex_::TypeManager::GetInst().GetIdFromType<MemFunctionWrapper<F>::ReturnType>() ),
            objectType_( Flex_::TypeManager::GetInst().GetIdFromType<MemFunctionWrapper<F>::ThisType>() ),
            returnIsValue_( Detail_::IsValueType<typename MemFunctionWrapper<F>::ReturnType>::value )
        {	}

        void Call(AnyPtr const& thisPtr, AnyParams const& params, AnyPtr& returnVal) const;
        void operator() (AnyPtr const& thisPtr, AnyParams const& params, AnyPtr& returnVal) const;

        int GetArity() const;

        typedef std::vector<TypeId> ParamTypes;
        // maybe switch to begin/end pair
        ParamTypes const& GetParamTypes() const;

        TypeId GetObjectType() const;

        TypeId GetReturnType() const;

        bool ReturnsByValue() const;

    private:
        typedef boost::function<void (AnyPtr const&, AnyParams const&, AnyPtr& )> BoostAnyFunction;
        BoostAnyFunction function_;

        int arity_;
        ParamTypes paramTypes_;
        TypeId returnType_, objectType_;
        bool returnIsValue_;
    };

    template <typename F>
    MemberFunction makeAnyFunction(F func)
    {
        return MemberFunction( MemFunctionWrapper<F>(func) );
    }

}

