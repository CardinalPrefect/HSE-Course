#include <cstdlib>
#include <utility>

#pragma once

namespace task {

template <typename Head, typename... Tail>
struct TypeList {
    using head = Head;
    using tail = TypeList<Tail...>;
};

template <unsigned int Idx, class TList>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<0, TypeList<Head, Tail...>> {
    using result = Head;
};

template <unsigned int Idx, typename Head, typename... Tail>
struct TypeAt<Idx, TypeList<Head, Tail...>> {
    using result = typename TypeAt<Idx - 1, TypeList<Tail...>>::result;
};

template <std::size_t I>
struct InPlaceIndex {
    explicit InPlaceIndex() = default;
};

template <std::size_t I>
constexpr InPlaceIndex<I> kInPlaceIndex{};

template <std::size_t Idx, typename... Type>
union Union;

template <std::size_t Idx>
union Union<Idx> {};

template <std::size_t Idx, typename T, typename... Type>
union Union<Idx, T, Type...> {
public:
    Union() : tail() {
        if (!std::is_trivially_constructible_v<T>) {
            new (&head) T();
        }
    }

    ~Union() {
        if (!std::is_trivially_destructible_v<T>) {
            head.~T();
        }
    }

public:
    T head;
    Union<Idx + 1, Type...> tail;

    friend struct UnionHelper;
};

struct UnionHelper {
    template <typename U>
    static constexpr auto&& GetAlt(U&& v, InPlaceIndex<0>) {
        return std::forward<U>(v).head;
    }

    template <typename U, std::size_t Idx>
    static constexpr auto&& GetAlt(U&& v, InPlaceIndex<Idx>) {
        return GetAlt(std::forward<U>(v).tail, kInPlaceIndex<Idx - 1>);
    }
};

struct VariantHelper {
    template <std::size_t Idx, typename T>
    static constexpr auto&& GetAlt(T&& v) {
        return UnionHelper::GetAlt(std::forward<T>(v).data_, kInPlaceIndex<Idx>);
    }
};

struct AssignUnion {
    template <std::size_t Idx, typename U, std::size_t UnIdx, typename Head, typename... Tail>
    static void AssignHelper(U&& value, InPlaceIndex<0>, Union<UnIdx, Head, Tail...>& u) {
        u.head = value;
    }

    template <std::size_t Idx, typename U, std::size_t UnIdx, typename Head, typename... Tail>
    static void AssignHelper(U&& value, InPlaceIndex<Idx>, Union<UnIdx, Head, Tail...>& u) {
        AssignUnion::AssignHelper<Idx - 1>(std::forward<U>(value), kInPlaceIndex<Idx - 1>, u.tail);
    }
};

const static std::size_t kNotFound = -1;
const static std::size_t kAmbiguity = kNotFound - 1;

constexpr std::size_t ProcessBackward(std::size_t i, std::size_t res, const bool* found,
                                      const bool* found_convertible) {
    if (res == kAmbiguity) {
        return res;
    }

    if (found[i]) {
        if (res == kNotFound || !(!found[res] && found_convertible[res])) {
            return i;
        }
        return kAmbiguity;
    }

    if (res == kNotFound && found_convertible[i]) {
        return i;
    }

    return res;
}

template <std::size_t SizeofFound>
constexpr std::size_t ProcessForward(std::size_t currnet, const bool (&found)[SizeofFound],
                                     const bool (&convertible)[SizeofFound]) {
    if (currnet == SizeofFound) {
        return kNotFound;
    }
    return ProcessBackward(currnet, ProcessForward(currnet + 1, found, convertible), found, 
                           convertible);
}

template <typename TargetType, typename... Type>
struct FindExactlyOneChecked {
    constexpr static bool kFound[sizeof...(Type)] = {std::is_same<TargetType, Type>::value...};
    constexpr static bool kFoundConvertible[sizeof...(Type)] = {
        std::is_convertible<TargetType, Type>::value...};
    constexpr static std::size_t kValue = ProcessForward(0, kFound, kFoundConvertible);

    static_assert(kValue != kNotFound);
    static_assert(kValue != kAmbiguity);
};

template <typename T>
struct FindExactlyOneChecked<T> {
    static_assert(!std::is_same<T, T>::value);
};

template <typename TargetType, typename... Type>
struct FindExactlyOne : public FindExactlyOneChecked<TargetType, Type...> {};

template <typename... Type>
class Variant;

template <size_t Idx, typename T>
struct VariantAlternative;

template <size_t Idx, typename T>
using variant_alternative_t = typename VariantAlternative<Idx, T>::type;

template <size_t Idx, typename... Type>
struct VariantAlternative<Idx, Variant<Type...>> {
    using type = typename TypeAt<Idx, TypeList<Type...>>::result;
};

template <typename... Type>
class Variant {
public:
    // Special member functions
    constexpr Variant() noexcept;

    template <typename T, std::size_t Pos = FindExactlyOne<T, Type...>::kValue>
    Variant& operator=(T&& t) noexcept;

private:
    Union<0, Type...> data_;

    friend VariantHelper;
};

template <typename... Type>
constexpr Variant<Type...>::Variant() noexcept {
}

template <typename... Type>
template <typename T, std::size_t Pos>
Variant<Type...>& Variant<Type...>::operator=(T&& t) noexcept {
    AssignUnion::AssignHelper<Pos>(std::forward<T>(t), kInPlaceIndex<Pos>, data_);
    return *this;
}

template <std::size_t Idx, typename T>
auto&& GenericGet(T&& v) {
    return VariantHelper::GetAlt<Idx>(std::forward<T>(v));
}

// Non-member functions
template <size_t I, typename... Type>
constexpr const variant_alternative_t<I, Variant<Type...>>& Get(Variant<Type...>& v) {
    return GenericGet<I>(v);
}

template <size_t I, typename... Type>
constexpr variant_alternative_t<I, Variant<Type...>>&& Get(Variant<Type...>&& v) {
    return GenericGet<I>(v);
}

template <typename T, typename... Type>
constexpr const T& Get(Variant<Type...>& v) {
    return GenericGet<FindExactlyOne<T, Type...>::kValue>(v);
}

template <typename T, typename... Type>
constexpr T&& Get(Variant<Type...>&& v) {
    return GenericGet<FindExactlyOne<T, Type...>::kValue>(v);
}

};  // namespace task