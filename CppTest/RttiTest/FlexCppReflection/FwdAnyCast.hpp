/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include "TypeId.hpp"

namespace Flex_
{
    class AnyPtr;

    namespace Detail_
    {
        template <typename ValueType>
        struct DoAnyCast
        {
            static ValueType* cast(AnyPtr const& op, TypeId valueId);
        };

        template <typename ValueType>
        struct DoAnyCast<ValueType const>
        {
            static ValueType const* cast(AnyPtr const& op, TypeId valueId);
        };
    }
}
