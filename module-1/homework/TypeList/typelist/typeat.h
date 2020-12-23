#pragma once

#include "typelist.h"

template<typename TList, unsigned int index>
struct TypeAt;

template<typename Head, typename Tail, unsigned int index>
struct TypeAt<TypeList<Head, Tail>, index> {
    using TargetType = typename TypeAt<Tail, index - 1>::TargetType;
};

template<typename Head, typename Tail>
struct TypeAt<TypeList<Head, Tail>, 0> {
    using TargetType = Head;
};

template<unsigned int index>
struct TypeAt<NullType, index> {
    using TargetType = NullType;
};
