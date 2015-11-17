#ifndef STREAM9_APPLY_HPP
#define STREAM9_APPLY_HPP

#include "index_sequence.hpp"
#include "tuple.hpp"

#include <tuple>
#include <type_traits>

namespace stream9 {

template<typename Fn, typename T, size_t... index>
auto apply_impl(Fn &&func, T&& tuple, index_sequence<index...>)
    -> decltype(func(std::get<index>(tuple)...))
{
    return func(std::get<index>(tuple)...);
}

template<typename Fn, typename T>
auto apply(Fn&& func, T&& tuple)
    -> decltype(apply_impl(std::forward<Fn>(func), std::forward<T>(tuple),
                                                   make_tuple_index_t<T>()))
{
    return apply_impl(std::forward<Fn>(func), std::forward<T>(tuple),
                                              make_tuple_index_t<T>());
}

} // namespace stream9

#endif // STREAM9_APPLY_HPP
