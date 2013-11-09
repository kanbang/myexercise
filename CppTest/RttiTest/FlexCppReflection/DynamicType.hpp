/***************************************************************************
 *   Copyright (C) 2008 by tyler shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include "TypeInfo.hpp"

namespace Flex_
{

    class DynamicType
    {
    public:
        DynamicType();
        virtual ~DynamicType();

        void SetType(TypeId type);
        TypeId GetType() const;

    private:
        TypeId dynTypeId;
    };

}

