#pragma once

#include <vector>

#include "AnyParamCast.hpp"

namespace Flex_
{

    class AnyPtr;

    class AnyParams
    {
    public:
        AnyPtr const& at(size_t i) const;

        void addParam(AnyPtr const& p);

        void clear();

        size_t size() const;

        template <typename T>
        typename ParamCastResult<T>::type retrieveParam(size_t i) const
        {
            return AnyParamCast<T>( params.at(i) );
        }

    private:
        typedef std::vector<AnyPtr> AnyVector;
        AnyVector params;
    };

}
