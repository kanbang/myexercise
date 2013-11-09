#include "MemberFunctions.hpp"

#include <cassert>

#include "MemberFunction.hpp"

namespace Flex_
{

    MemberFunctions& MemberFunctions::AddFunction( std::string const& name, MemberFunction const& fn )
    {
        assert( !HasFunction( name ) &&
                "Overwriting function name?! Overloads are confusing! (call it something else)" );

        functions_.insert( std::make_pair( name, fn ) );
        return *this;
    }

    bool MemberFunctions::Empty() const
    {
        return functions_.empty();
    }

    MemberFunctions::FunctionRange MemberFunctions::GetAllFunctions() const
    {
        return FunctionRange( functions_.begin(), functions_.end() );
    }

    bool MemberFunctions::HasFunction( std::string const& name ) const
    {
        return functions_.find( name ) != functions_.end();
    }

    MemberFunction const& MemberFunctions::GetFunction( std::string const& name ) const
    {
        assert( HasFunction( name ) &&
                "Getting non-existent function?! Bold move... let's see if it pays off" );
        return functions_.find( name )->second;
    }

}

