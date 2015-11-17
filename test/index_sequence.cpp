#include <stream9/index_sequence.hpp>

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

void func()
{
    std::cout << "\n";
}

template<typename T, typename... Args>
void func(T v, Args... args)
{
    std::cout << v << " ";
    func(args...);
}

template<typename T, size_t... index>
void print_tuple(T v, stream9::index_sequence<index...>)
{
    func(std::get<index>(v)...);
}

TEST(index_sequence, make_index_sequence)
{
    std::ostringstream oss;

    auto* const orig = std::cout.rdbuf(oss.rdbuf());

    const auto &tuple = std::make_tuple(10, 3.14, true, "foo");
    const auto N = std::tuple_size<
                    std::decay<decltype(tuple)>::type>::value;

    print_tuple(tuple, stream9::make_index_sequence<N>());

    EXPECT_EQ("10 3.14 1 foo \n", oss.str());

    std::cout.rdbuf(orig);
}
