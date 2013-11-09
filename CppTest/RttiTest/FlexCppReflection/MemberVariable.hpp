#pragma once

#include <string>
#include <map>

#include <boost/mpl/list.hpp>

namespace Flex_
{

    class AnyPtr;

    class MemberVariable
    {
    public:
        virtual ~MemberVariable() {}

        virtual AnyPtr GetPointer(AnyPtr const& object) const = 0;
    };

    typedef std::map<std::string, MemberVariable const*> MemberVariablePtrMap;

    template <typename T>
    struct MemberVariableList
    {
        typedef T Type;
        typedef boost::mpl::list<> VariableTypeList;
        static MemberVariablePtrMap GetVariablePtrs()
        {
            return MemberVariablePtrMap();
        }
    };

}
