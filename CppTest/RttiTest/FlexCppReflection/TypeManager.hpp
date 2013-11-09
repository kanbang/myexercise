/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "TypeInfo.hpp"
#include "PlainType.hpp"

#define FLEX_TYPES (::Flex_::TypeManager::GetInst())

namespace Flex_
{

    class MemberFunctions;

    class TypeManager
    {
    public:
        static TypeManager& GetInst();
        void Shutdown();

        TypeInfo CreateTypeInfo();

        int TotalTypesCreated() const;

        template <typename T>
        TypeId RegisterType(TypeInfo const& info)
        {
            return registerType(typeid( typename StripAllCv<T>::type ), info);
        }

        void SetMemberFunctions(TypeId typeId, MemberFunctions const& functions);

        void AddTypeImplementation(TypeId baseId, TypeId derivedId);

        void AddTypeAlias(TypeId typeId, std::string const& aliasName);

        TypeInfo const& GetInfoFromId(TypeId typeId) const;

        template <typename T>
        TypeId GetIdFromType() const
        {
            return GetIdFromStdInfo( typeid(typename StripAllCv<T>::type) );
        }
        TypeId GetIdFromStdInfo(std::type_info const& info) const;

        TypeId GetIdFromName(std::string const& name) const;

    private:

        TypeManager();
        TypeManager(TypeManager const&);
        TypeManager& operator=(TypeManager const&);

        TypeId registerType(std::type_info const& typeId, TypeInfo info);
        TypeId createNewTypeId();

        struct Impl;
        std::auto_ptr<Impl> impl_;
    };

}

namespace Flex_
{

    template <typename T>
    T* TypeInfo::Create() const
    {
        void* obj = create();

        const TypeId ptrType = TypeManager::GetInst().GetIdFromType<T>();
        if(ptrType == GetId()) // returning as my type
            return static_cast<T*>(obj);
        else	// returning as ptr type, ensure correct cast
            return static_cast<T*>( cast( obj, ptrType ) );
    }

    template <typename T>
    T* TypeInfo::Clone(T const& ptr) const
    {
        void* obj = clone( CastToMe(&ptr) );

        const TypeId ptrType = TypeManager::GetInst().GetIdFromType<T>();
        if(ptrType == GetId())
            return static_cast<T*>(obj);
        else
            return static_cast<T*>( cast( obj, ptrType ) );
    }

    template <typename T>
    void TypeInfo::Destroy(T* ptr) const
    {
        return destroy( CastToMe(ptr) );
    }

    template <typename BaseType>
    void* TypeInfo::CastToMe(BaseType* otherPtr) const
    {
        const TypeId ptrType = TypeManager::GetInst().GetIdFromType<BaseType>();

        if(ptrType == GetId()) // already this type
            return otherPtr;

        return TypeManager::GetInst().GetInfoFromId(ptrType).cast( otherPtr, GetId() );
    }

    template <typename BaseType>
    void const* TypeInfo::CastToMe(BaseType const* otherPtr) const
    {
        return CastToMe( const_cast<BaseType*>(otherPtr) );
    }

}

