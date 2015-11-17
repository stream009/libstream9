#ifndef STREAM9_TUPLE_HPP
#define STREAM9_TUPLE_HPP

#include "index_sequence.hpp"

#include <tuple>
#include <type_traits>

namespace stream9 {

template<typename T>
struct make_tuple_index
{
    static constexpr size_t N =
                    std::tuple_size<typename std::decay<T>::type>::value;

    using type = make_index_sequence<N>;
};

template<typename T>
using make_tuple_index_t = typename make_tuple_index<T>::type;

} // namespace stream9

#endif // STREAM9_TUPLE_HPP
