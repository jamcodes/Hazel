#pragma once

#include <type_traits>

namespace Hazel {
template <typename E>
struct enableEnumOperators {
    static constexpr bool value{false};
};

template <typename IntegralType, typename E>
[[nodiscard]] inline constexpr std::enable_if_t<enableEnumOperators<E>::value, IntegralType>
toInteger(const E e) noexcept
{
    static_assert(std::is_integral_v<IntegralType>, "toInteger may only cast to integral types");
    return static_cast<IntegralType>(e);
}

template <typename E>
std::enable_if_t<enableEnumOperators<E>::value, E> operator|(E const lhs, E const rhs) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

template <typename E>
std::enable_if_t<enableEnumOperators<E>::value, E> operator&(E const lhs, E const rhs) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
}

// TODO: Implement other operators as needed

}  // namespace Hazel
