#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct IndexOf;

template<typename Head, typename Tail, typename TargetType>
struct IndexOf<TypeList<Head, Tail>, TargetType> {
    static const int pos = IndexOf<Tail, TargetType>::pos == -1? -1 : IndexOf<Tail, TargetType>::pos + 1;
};

template<typename TargetType, typename Tail>
struct IndexOf<TypeList<TargetType, Tail>, TargetType> {
    static const int pos = 0;
};

template<typename TargetType>
struct IndexOf<NullType, TargetType> {
    static const int pos = -1;
};
