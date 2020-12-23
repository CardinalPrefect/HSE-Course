#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct EraseAll;

template<typename Head, typename Tail, typename TargetType>
struct EraseAll<TypeList<Head, Tail>, TargetType> {
    using NewTypeList = TypeList<Head, typename EraseAll<Tail, TargetType>::NewTypeList>;
};

template<typename Tail, typename TargetType>
struct EraseAll<TypeList<TargetType, Tail>, TargetType> {
    using NewTypeList = typename EraseAll<Tail, TargetType>::NewTypeList;
};

template<typename TargetType>
struct EraseAll<NullType, TargetType> {
    using NewTypeList = NullType;
};
