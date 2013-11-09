/***************************************************************************
 *   Copyright (C) 2008 by Tyler Shaub   *
 *   tyler.shaub@gmail.com   *
 *                                                                         *
 *   See COPYING	*
 ***************************************************************************/

#pragma once

#include <string>
#include <set>
#include <memory>

#include <boost/function.hpp>

#include "TypeId.hpp"
#include "FwdAnyCast.hpp"
#include "MemberVariable.hpp"

namespace Flex_
{

    typedef boost::function<void* ()> CreateFn;
    typedef boost::function<void (void*)> DestroyFn;
    typedef boost::function<void* (void const*)> CloneFn;

    typedef void* (*CastFn)(void*);

    class AnyPtr;
    class MemberFunctions;
    class MemberVariables;

    class TypeInfo
    {
    public:

        explicit TypeInfo(TypeId id);

        TypeInfo(TypeInfo const& cpy);
        TypeInfo& operator=(TypeInfo const& rhs);

        ~TypeInfo();

        void swap(TypeInfo& other);

        void SetCreationFunctions(CreateFn const& create,
                                  CloneFn const& clone,
                                  DestroyFn const& destruct,
                                  DestroyFn const& destroy);

        void SetFunctions(MemberFunctions const& fns);
        MemberFunctions const& GetFunctions() const;

        void SetVariables(MemberVariablePtrMap const& vars);
        MemberVariables const& GetVariables() const;

        void SetSize(size_t size);
        void SetName(char const* name);
        void SetReferenceType(TypeId id); // non-pointer-type id

        std::string const& GetName() const;
        size_t GetSize() const;

        TypeId GetId() const;
        // NOTE: consider ( implicit :/ ) conversion to id
        // this generally makes sense and allows for operator<, ==, etc

        TypeId GetReferenceTypeId() const; // non-pointer-type id

        const bool ImplementsInterface(TypeId faceId) const;

        typedef std::set<TypeId> TypeSet;
        TypeSet const& GetInterfaces() const;
        TypeSet const& GetImplementations() const;

        void AddInterface(TypeInfo const& base);
        void AddImplementation(TypeId impl);

        void AddCast(TypeId toType, CastFn const& cast) const;

        template <typename T>
        T* Create() const;

        template <typename T>
        T* Clone(T const& ptr) const;

        template <typename T>
        void Destroy(T* ptr) const;

        AnyPtr CreateAny() const;
        AnyPtr CopyAny(AnyPtr const& ptr) const;
        void DestroyAny(AnyPtr ptr) const;


        // safely cast from a base pointer to the this-type's pointer
        // WARNING: this must return a void* to ensure the correct address
        template <typename BaseType>
        void* CastToMe(BaseType* basePtr) const;

        template <typename BaseType>
        void const* CastToMe(BaseType const* basePtr) const;

        // TMP DEBUG:
        void Print() const;

    private:

        void setType(TypeId id);

        void* cast(void* mePtr, TypeId toType) const;
        void const* cast(void const* mePtr, TypeId toType) const;

        void* create() const;
        void* clone(void const*) const;
        void destroy(void*) const;

        // external casts
        template<typename ValueType>
        friend struct Detail_::DoAnyCast;

        struct Impl;
        std::auto_ptr<Impl> impl_; // custom alloc prevents const auto_ptr or scoped_ptr :(
    };

    inline void swap(TypeInfo& lhs, TypeInfo& rhs)
    {
        lhs.swap(rhs);
    }

} // Flex_

// include type manager for template function impls
#include "TypeManager.hpp"

