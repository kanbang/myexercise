/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#include <boost/lexical_cast.hpp>

#include "AnyPtr.hpp"

#include "TypeInfo.hpp"

namespace Flex_
{

    AnyPtr::AnyPtr()
        : content_( 0 ), constContent_( 0 ), typeInfo_( 0 )
    {
    }

    AnyPtr::AnyPtr( AnyPtr const& other )
        : content_( other.content_ ), constContent_( other.constContent_ ), typeInfo_( other.typeInfo_ )
    {
    }

    AnyPtr::~AnyPtr()
    {
    }

    AnyPtr& AnyPtr::swap( AnyPtr& rhs )
    {
        std::swap( content_, rhs.content_ );
        std::swap( constContent_, rhs.constContent_ );
        std::swap( typeInfo_, rhs.typeInfo_ );
        return *this;
    }

    AnyPtr& AnyPtr::operator=( AnyPtr const& rhs )
    {
        AnyPtr( rhs ).swap( *this );
        return *this;
    }

    std::string AnyPtr::ToString() const
    {
        std::ostringstream oss;
        oss << GetTypeId() << " " << content_ << " " << constContent_;
        return oss.str();
    }

    void AnyPtr::Deserialize( std::string const& str )
    {
        std::istringstream iss;
        iss.str( str );

        void* constStr = 0;  // iss does not define a void const*& operator>> use this tmp
        TypeId id = INVALID_TYPE;
        iss >> id >> content_ >> constStr;

        typeInfo_ = &TypeManager::GetInst().GetInfoFromId( id );

        constContent_ = constStr;
    }

    AnyPtr AnyPtr::DereferenceDoublePtr() const
    {
        AnyPtr result( *this );

        if( !Empty() && typeInfo_->GetReferenceTypeId() != typeInfo_->GetId() )
        {
            result.content_ = content_ ? ( *static_cast<void**>( content_ ) ) : NULL;
            result.constContent_ = *static_cast<void const* const*>( constContent_ ); // no need to check (!Empty)

            result.typeInfo_ =  &TypeManager::GetInst().GetInfoFromId( typeInfo_->GetReferenceTypeId() );
        }

        return result;
    }

    bool AnyPtr::Empty() const
    {
        return !constContent_;
    }

    bool AnyPtr::Mutable() const
    {
        // content set AND not just const version
        return content_ != NULL && constContent_ != NULL;
    }

    AnyPtr& AnyPtr::SetConst()
    {
        content_ = NULL;
        return *this;
    }

    TypeInfo const& AnyPtr::GetType() const
    {
        assert( typeInfo_ != NULL );
        return *typeInfo_;
    }

    TypeId AnyPtr::GetTypeId() const
    {
        return typeInfo_ ? typeInfo_->GetId() : INVALID_TYPE;
    }

    void* AnyPtr::getUnsafePtr()
    {
        return content_;
    }

    void const* AnyPtr::getUnsafePtr() const
    {
        return constContent_;
    }

}
