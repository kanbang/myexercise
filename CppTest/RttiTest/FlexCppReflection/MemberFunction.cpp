#include "MemberFunction.hpp"

namespace Flex_
{

    MemberFunction::MemberFunction() : arity_( -1 ) {}

    void MemberFunction::Call( AnyPtr const& thisPtr, AnyParams const& params, AnyPtr& returnVal ) const
    {
        function_( thisPtr, params, returnVal );
    }
    void MemberFunction::operator() ( AnyPtr const& thisPtr, AnyParams const& params, AnyPtr& returnVal ) const
    {
        Call( thisPtr, params, returnVal );
    }

    int MemberFunction::GetArity() const
    {
        return arity_;
    }

    MemberFunction::ParamTypes const& MemberFunction::GetParamTypes() const
    {
        return paramTypes_;
    }

    TypeId MemberFunction::GetObjectType() const
    {
        return objectType_;
    }

    TypeId MemberFunction::GetReturnType() const
    {
        return returnType_;
    }

    bool MemberFunction::ReturnsByValue() const
    {
        return returnIsValue_;
    }

}
