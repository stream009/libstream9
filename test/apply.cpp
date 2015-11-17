#include <stream9/apply.hpp>

#include <tuple>

#include <gtest/gtest.h>

int printer(int i, double d, bool b, const char *s)
{
    std::cout << i << " "
              << d << " "
              << b << " "
              << s << "\n";
    return 100;
}

TEST(apply, apply)
{
    using stream9::apply;

    std::ostringstream oss;

    auto* const orig = std::cout.rdbuf(oss.rdbuf());

    const auto &tuple = std::make_tuple(10, 3.14, true, "foo");

    std::cout << apply(printer, tuple) << "\n";

    EXPECT_EQ("10 3.14 1 foo\n100\n", oss.str());

    oss.str("");
    std::cout <<
        apply([](int i, double d, bool b, const char *s) {
            std::cout << i << " "
                      << d << " "
                      << b << " "
                      << s << "\n";
            return 200;
        }, tuple) << "\n";

    EXPECT_EQ("10 3.14 1 foo\n200\n", oss.str());

    std::cout.rdbuf(orig);
}
