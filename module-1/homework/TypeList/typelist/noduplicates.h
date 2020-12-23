#pragma once

#include "eraseall.h"
#include "typelist.h"

template<typename TList>
struct NoDuplicates;

template<typename Head, typename Tail>
struct NoDuplicates<TypeList<Head, Tail>> {
    using NewTypeList = TypeList<Head, typename NoDuplicates<typename EraseAll<Tail, Head>::NewTypeList>::NewTypeList>;
};

template<>
struct NoDuplicates<NullType> {
    using NewTypeList = NullType;
};
