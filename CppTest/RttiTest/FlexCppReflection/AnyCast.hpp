/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include <cassert>

#include <boost/type_traits.hpp>

#include "TypeInfo.hpp"
#include "TypeManager.hpp"

#include "PlainType.hpp"
#include "AnyPtr.hpp"

namespace Flex_
{
    namespace Detail_
    {

        template <typename ValueType>
        ValueType* DoAnyCast<ValueType>::cast(AnyPtr const& op, TypeId valueId)
        {
            assert( op.content_ && "Attempting to cast away const-ness!" );
            return static_cast<ValueType*>(op.typeInfo_->cast(op.content_, valueId) );
        }

        template <typename ValueType>
        ValueType const* DoAnyCast<ValueType const>::cast(AnyPtr const& op, TypeId valueId)
        {
            return static_cast<ValueType const*>(op.typeInfo_->cast(op.constContent_, valueId) );
        }
    }

    template<typename ValueType>
    typename Detail_::StripRefPtr<ValueType>::type* AnyCast( AnyPtr const& op )
    {
        if(op.Empty()) return 0;

        typedef typename Detail_::StripRefPtr<ValueType>::type PtrValueType;

        const TypeId valueId = TypeManager::GetInst().GetIdFromType< PtrValueType >();

        assert(valueId != INVALID_TYPE && "Cannot cast Any to unknown Type");

        return Detail_::DoAnyCast< PtrValueType >::cast(op, valueId);
    }

}
