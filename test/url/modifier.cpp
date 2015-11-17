#include <stream9/url.hpp>

#include <boost/utility/string_ref.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

#include <gtest/gtest.h>

namespace stream9 {

void PrintTo(const url &u, std::ostream *os)
{
    *os << u;
}

} // namespace stream9

using url = stream9::url;
using namespace stream9::literal;

TEST(modifier, scheme)
{
    { // string literal
        url u { "http://www.google.com" };

        ASSERT_NO_THROW(u.scheme("https"));
        EXPECT_EQ(u, "https://www.google.com"_u);
    }
    { // char*
        url u { "http://www.google.com" };
        const char* const scheme = "https";

        ASSERT_NO_THROW(u.scheme(scheme));
        EXPECT_EQ(u, "https://www.google.com"_u);
    }
    { // char[]
        url u { "http://www.google.com" };
        const char scheme[] = "https";

        ASSERT_NO_THROW(u.scheme(scheme));
        EXPECT_EQ(u, "https://www.google.com"_u);
    }
    { // std::string
        url u { "http://www.google.com" };
        std::string scheme { "https" };

        ASSERT_NO_THROW(u.scheme(scheme));
        EXPECT_EQ(u, "https://www.google.com"_u);
    }
    { // boost::string_ref
        url u { "http://www.google.com" };
        boost::string_ref scheme { "https" };

        ASSERT_NO_THROW(u.scheme(scheme));
        EXPECT_EQ(u, "https://www.google.com"_u);
    }
}

TEST(modifier, host)
{
    url u { "http://www.google.com/search" };

    ASSERT_NO_THROW(u.host("www.twitter.com"));
    EXPECT_EQ(u, "http://www.twitter.com/search"_u);
}

TEST(modifier, port)
{
    url u { "http://www.google.com/search" };

    ASSERT_NO_THROW(u.port(80));
    EXPECT_EQ("http://www.google.com:80/search"_u, u);

    ASSERT_NO_THROW(u.port(boost::none));
    EXPECT_EQ("http://www.google.com/search"_u, u);
}

TEST(modifier, path)
{
    url u { "http://www.google.com/" };

    ASSERT_NO_THROW(u.path("/search"));
    EXPECT_EQ("http://www.google.com/search"_u, u);

    ASSERT_NO_THROW(u.path(boost::none));
    EXPECT_EQ("http://www.google.com"_u, u);
}

TEST(modifier, query)
{
    url u { "http://www.google.com/search?q=bar" };

    ASSERT_NO_THROW(u.query("q=foo"));
    EXPECT_EQ("http://www.google.com/search?q=foo"_u, u);

    ASSERT_NO_THROW(u.query(boost::none));
    EXPECT_EQ("http://www.google.com/search"_u, u);
}

TEST(modifier, fragment)
{
    url u { "http://www.google.com/search#foo" };

    ASSERT_NO_THROW(u.fragment("bar"));
    EXPECT_EQ("http://www.google.com/search#bar"_u, u);

    ASSERT_NO_THROW(u.fragment(boost::none));
    EXPECT_EQ("http://www.google.com/search"_u, u);
}

TEST(modifier, user_info)
{
    url u { "http://user:password@www.google.com" };

    ASSERT_NO_THROW(u.user_info("foo:bar"));
    EXPECT_EQ("http://foo:bar@www.google.com"_u, u);

    ASSERT_NO_THROW(u.user_info(boost::none));
    EXPECT_EQ("http://www.google.com"_u, u);
}
