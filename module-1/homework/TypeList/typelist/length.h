#pragma once

#include "typelist.h"
#include <iostream> // for size_t

template<typename TList>
struct Length;

template <typename Head, typename Tail>
struct Length<TypeList<Head, Tail> > {
    static const std::size_t length = 1 + Length<Tail>::length;
};

template<>
struct Length<NullType> {
    static const int length = 0;
};
