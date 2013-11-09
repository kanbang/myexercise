/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include "TypeManager.hpp"
#include "TypeInfo.hpp"
#include "TypeInfoHelpers.hpp"

namespace Flex_
{

// prefer StaticInfo<T>::GetTypeId ONLY if TypeId is all that's needed,
// AND code is pre-main AND convenience macros were used
    template <typename T>
    TypeInfo const& GetInfoFromType()
    {
        TypeManager const& f = TypeManager::GetInst();
        return f.GetInfoFromId( f.GetIdFromType<T>() );
    }

    template <typename T>
    TypeInfo const& GetInfoFromObj(T const& obj)
    {
        TypeManager const& f = TypeManager::GetInst();
        return f.GetInfoFromId( f.GetIdFromStdInfo(typeid(obj)) );
    }

    template <typename T>
    TypeInfo const& GetInfoFromObj(T const* obj)
    {
        if(obj)
            return GetInfoFromObj(*obj);
        else
            return TypeInfo(INVALID_TYPE);
    }

    bool ImplementsFunction(TypeId type, std::string const& functionName);

    bool ImplementsInterface(TypeId objType, TypeId interfaceType);

    template <typename T>
    bool ImplementsInterface(T const& obj, int interfaceId)
    {
        return GetInfoFromObj(obj).ImplementsInterface(interfaceId);
    }

    template <typename Face, typename Obj>
    bool ImplementsStatic(Obj const& o)
    {
        return ImplementsInterface<Obj>(o, TypeManager::GetInst().GetIdFromType<Face>() );
    }

    template <typename Base>
    void* GetMostDerivedAddress(Base* ptr)
    {
        return GetInfoFromObj(*ptr).CaseToMe(ptr);
    }

    template <typename Base>
    void const* GetMostDerivedAddress(Base const* ptr)
    {
        return GetInfoFromObj(*ptr).CaseToMe(ptr);
    }

    template <typename T>
    T* Copy(T const& obj)
    {
        return GetInfoFromObj(obj).Clone(obj);
    }

    template <typename T>
    void Destroy(T* obj)
    {
        if(obj)
            GetInfoFromObj(*obj).Destroy(obj);
    }

    template <typename T>
    T* Create(TypeId id)
    {
        return TypeManager::GetInst().GetInfoFromId(id).Create<T>();
    }

    template <typename T>
    T* Create(const std::string& name)
    {
        return Create<T>(TypeManager::GetInst().GetIdFromName(name));
    }



}

