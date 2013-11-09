/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#include "TypeManagerHelpers.hpp"

#include "MemberFunctions.hpp"

namespace Flex_
{

    bool ImplementsFunction( TypeId type, std::string const& functionName )
    {
        return TypeManager::GetInst().GetInfoFromId( type ).GetFunctions().HasFunction( functionName );
    }

    bool ImplementsInterface( TypeId objType, TypeId interfaceType )
    {
        return TypeManager::GetInst().GetInfoFromId( objType ).ImplementsInterface( interfaceType );
    }


}

