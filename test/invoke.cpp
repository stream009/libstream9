#include <stream9/invoke.hpp>

#include <iostream>

#include <gtest/gtest.h>

namespace detail {

template<typename T>
void func_impl(T&& value)
{
    std::cout << value;
}

template<typename T, typename... Args>
void func_impl(T&& value, Args&&... args)
{
    std::cout << value << " ";
    func_impl(std::forward<Args>(args)...);
}

} // namespace detail

template<typename... Args>
void func(Args&&... args)
{
    detail::func_impl(std::forward<Args>(args)...);
}

TEST(invoke, variadic_function_template)
{
    std::ostringstream oss;
    auto* const orig = std::cout.rdbuf(oss.rdbuf());

    stream9::invoke(func<int, int, int>, 10, 20, 30);

    std::cout.rdbuf(orig);

    EXPECT_EQ("10 20 30", oss.str());
}
