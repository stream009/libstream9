#include <stream9/url.hpp>

#include <boost/utility/string_ref.hpp>

#include <gtest/gtest.h>

using url = stream9::url;

TEST(query, size)
{
    url u { "http://www.google.com" };
    EXPECT_EQ(21, u.size());
}

TEST(query, front)
{
    url u { "http://www.google.com" };
    EXPECT_EQ('h', u.front());
}

TEST(query, back)
{
    url u { "http://www.google.com" };
    EXPECT_EQ('m', u.back());
}

TEST(query, begin)
{
    url u { "http://www.google.com" };

    EXPECT_EQ(u.front(), *u.begin());
    EXPECT_EQ(u.front(), *u.cbegin());
    EXPECT_EQ(u.back(),  *u.rbegin());
    EXPECT_EQ(u.back(),  *u.crbegin());
}

TEST(query, end)
{
    url u { "http://www.google.com" };
    const auto end = u.begin() + u.size();
    const auto rend = u.rbegin() + u.size();

    EXPECT_EQ(end,  u.end());
    EXPECT_EQ(end,  u.cend());
    EXPECT_EQ(rend, u.rend());
    EXPECT_EQ(rend, u.crend());
}

TEST(query, c_str)
{
    url u { "http://www.google.com" };

    EXPECT_STREQ("http://www.google.com", u.c_str());
}

TEST(query, to_string)
{
    const std::string str { "http://www.google.com" };
    url u { str };

    EXPECT_EQ(str, to_string(u)); // ADL
}
