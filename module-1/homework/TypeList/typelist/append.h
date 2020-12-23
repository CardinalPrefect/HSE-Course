#pragma once

#include "typelist.h"

template<typename TList, typename NewType>
struct Append;

template<typename Head, typename Tail, typename NewType>
struct Append<TypeList<Head, Tail>, NewType> {
    using NewTypeList = TypeList<Head, typename Append<Tail, NewType>::NewTypeList>;
};

template<typename NewType>
struct Append<NullType, NewType> {
    using NewTypeList = TypeList<NewType, NullType>;
};

template<>
struct Append<NullType, NullType> {
    using NewTypeList = NullType;
};
