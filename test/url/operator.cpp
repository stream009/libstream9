#include <stream9/url.hpp>

#include <functional>
#include <sstream>

#include <boost/utility/string_ref.hpp>

#include <gtest/gtest.h>

using url = stream9::url;

TEST(operator, equal_to_url)
{
    url u1 { "http://www.google.com" };
    url u2 { "http://www.google.com" };
    url u3 { "http://www.twitter.com" };

    EXPECT_EQ(u1, u2);
    EXPECT_NE(u1, u3);
    EXPECT_NE(u3, u1);
    EXPECT_EQ(u1, std::move(u2));
}

TEST(operator, equal_to_string)
{
    url u1 { "http://www.google.com" };
    std::string u2 { "http://www.google.com" };
    std::string u3 { "http://www.twitter.com" };

    EXPECT_EQ(u1, u2);
    EXPECT_NE(u1, u3);
    EXPECT_NE(u3, u1);
}

TEST(operator, equal_to_string_ref)
{
    url u1 { "http://www.google.com" };
    boost::string_ref u2 { "http://www.google.com" };
    boost::string_ref u3 { "http://www.twitter.com" };

    EXPECT_EQ(u1, u2);
    EXPECT_NE(u1, u3);
    EXPECT_NE(u3, u1);
}

TEST(operator, equal_to_c_str)
{
    url u1 { "http://www.google.com" };
    const char *u2 = "http://www.google.com";
    const char *u3 = "http://www.twitter.com";

    EXPECT_EQ(u1, u2);
    EXPECT_NE(u1, u3);
    EXPECT_NE(u3, u1);
}

TEST(operator, less_than_url)
{
    url u1 { "http://www.google.com/" };
    url u2 { "http://www.google.com/search" };
    url u3 { "http://www.google.com/" };

    EXPECT_LT(u1, u2);
    EXPECT_GT(u2, u1);
    EXPECT_LE(u1, u3);
    EXPECT_GE(u1, u3);
    EXPECT_LE(u3, u1);
    EXPECT_GE(u3, u1);
}

TEST(operator, less_than_string)
{
    url         u1 { "http://www.google.com/" };
    std::string u2 { "http://www.google.com/search" };
    std::string u3 { "http://www.google.com/" };

    EXPECT_LT(u1, u2);
    EXPECT_GT(u2, u1);
    EXPECT_LE(u1, u3);
    EXPECT_GE(u1, u3);
    EXPECT_LE(u3, u1);
    EXPECT_GE(u3, u1);
}

TEST(operator, less_than_string_ref)
{
    url               u1 { "http://www.google.com/" };
    boost::string_ref u2 { "http://www.google.com/search" };
    boost::string_ref u3 { "http://www.google.com/" };

    EXPECT_LT(u1, u2);
    EXPECT_GT(u2, u1);
    EXPECT_LE(u1, u3);
    EXPECT_GE(u1, u3);
    EXPECT_LE(u3, u1);
    EXPECT_GE(u3, u1);
}

TEST(operator, less_than_c_str)
{
    url         u1 { "http://www.google.com/" };
    const char *u2 { "http://www.google.com/search" };
    const char *u3 { "http://www.google.com/" };

    EXPECT_LT(u1, u2);
    EXPECT_GT(u2, u1);
    EXPECT_LE(u1, u3);
    EXPECT_GE(u1, u3);
    EXPECT_LE(u3, u1);
    EXPECT_GE(u3, u1);
}

TEST(operator, streaming)
{
    url u { "http://www.google.com" };
    std::ostringstream oss;

    oss << u;

    EXPECT_EQ(to_string(u), oss.str());
}

TEST(operator, swap)
{
    url u1 { "http://www.google.com" };
    url u2 { "http://www.twitter.com" };

    url u1_copy { u1 };
    url u2_copy { u2 };

    u1.swap(u2);
    EXPECT_EQ(u2_copy, u1);
    EXPECT_EQ(u1_copy, u2);

    std::swap(u1, u2);
    EXPECT_EQ(u1_copy, u1);
    EXPECT_EQ(u2_copy, u2);
}

TEST(operator, hash)
{
    url u1 { "http://www.google.com" };
    url u2 { "http://www.google.com" };
    url u3 { "http://www.twitter.com" };

    std::hash<url> hash;
    EXPECT_EQ(hash(u2), hash(u1));
    EXPECT_NE(hash(u3), hash(u1));
}

TEST(operator, literal)
{
    using namespace stream9::literal;

    url u1 { "http://www.google.com" };

    EXPECT_EQ(u1, "http://www.google.com"_u);
}
