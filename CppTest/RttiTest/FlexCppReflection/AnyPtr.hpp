/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include <string>
#include <algorithm>

#include "TypeInfo.hpp"
#include "TypeManager.hpp"
#include "FwdAnyCast.hpp"

namespace Flex_
{

    class AnyPtr
    {
    public:

        AnyPtr();

        template<typename ValueType>
        AnyPtr( ValueType* value )
            : content_( 0 ), constContent_( 0 ), typeInfo_( 0 )
        {
            if(value)
            {
                TypeId id = TypeManager::GetInst().GetIdFromStdInfo(typeid(*value));
                assert( id != INVALID_TYPE );

                typeInfo_ = &TypeManager::GetInst().GetInfoFromId(id);
                content_ = typeInfo_->CastToMe(value);
                constContent_ = content_;
            }
        }

        template<typename ValueType>
        AnyPtr( ValueType const* value )
            : content_( 0 ), constContent_( 0 ), typeInfo_( 0 )
        {
            if(value)
            {
                TypeId id = TypeManager::GetInst().GetIdFromStdInfo(typeid(*value));
                assert( id != INVALID_TYPE );

                typeInfo_ = &TypeManager::GetInst().GetInfoFromId(id);
                constContent_ = typeInfo_->CastToMe(value);
            }
        }

        AnyPtr( AnyPtr const& other );

        ~AnyPtr();

        AnyPtr& swap( AnyPtr& rhs );

        template<typename ValueType>
        AnyPtr& operator=( ValueType* rhs )
        {
            AnyPtr( rhs ).swap( *this );
            return *this;
        }

        AnyPtr& operator=( AnyPtr const& rhs );

        std::string ToString() const;
        void Deserialize(std::string const& str);

        // get a * from a ** (* to *)
        AnyPtr DereferenceDoublePtr() const;

        // explicit Empty function preferred over operator bool()
        bool Empty() const;

        // check to see if pointer is const or not
        bool Mutable() const;

        // convert this pointer to const, returns self
        AnyPtr& SetConst();

        TypeInfo const& GetType() const;
        TypeId GetTypeId() const;

    private:

        friend class TypeInfo;
        void* getUnsafePtr();
        void const* getUnsafePtr() const;

        AnyPtr(void* ptr, TypeInfo const* type)
            : content_(ptr), constContent_(ptr), typeInfo_(type)
        {
        }

        AnyPtr(void const* ptr, TypeInfo const* type)
            : content_(0), constContent_(ptr), typeInfo_(type)
        {
        }

        template<typename ValueType>
        friend struct Detail_::DoAnyCast;

        void* content_;
        void const* constContent_;

        TypeInfo const* typeInfo_;
    };

    struct SameType
    {
        TypeId type_;
        SameType(TypeId type) : type_(type) {}
        SameType(AnyPtr const& ptr) : type_(ptr.GetType().GetId()) {}
        bool operator() (AnyPtr const& p) const
        {
            return p.GetType().GetId() == type_;
        }
    };

}

