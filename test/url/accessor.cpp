#include <stream9/url.hpp>

#include <boost/utility/string_ref.hpp>

#include <gtest/gtest.h>

using url = stream9::url;

TEST(accessor, scheme)
{
    {
        url u { "http://www.google.com" };
        EXPECT_EQ("http", u.scheme());
    }
    {
        url u { "ftp://www.google.com" };
        EXPECT_EQ("ftp", u.scheme());
    }
    {
        url u { "https://www.google.com" };
        EXPECT_EQ("https", u.scheme());
    }
}

TEST(accessor, host)
{
    {
        url u { "http://www.google.com/search" };
        ASSERT_TRUE(!!u.host());
        EXPECT_EQ("www.google.com", *u.host());
    }
    {
        url u { "http://192.168.1.1:8080/index.php" };
        ASSERT_TRUE(!!u.host());
        EXPECT_EQ("192.168.1.1", *u.host());
    }
    {
        url u { "http://localhost:8080/index.php" };
        ASSERT_TRUE(!!u.host());
        EXPECT_EQ("localhost", *u.host());
    }
    {
        url u { "http://[2001:db8:1f70::999:de8:7648:6e8]:8080/" };
        ASSERT_TRUE(!!u.host());
        EXPECT_EQ("2001:db8:1f70::999:de8:7648:6e8", *u.host());
    }
#if 0 // http_parser_parse_url doesn't support this
    {
        url u { "mailto:foobar@gmail.com" };
        EXPECT_FALSE(!!u.host());
    }
#endif
}

TEST(accessor, port)
{
    {
        url u { "http://www.google.com/search" };
        EXPECT_FALSE(!!u.port());
    }
    {
        url u { "http://www.google.com:80/search" };
        ASSERT_TRUE(!!u.port());
        EXPECT_EQ(80, u.port().get());
    }
}

TEST(accessor, path)
{
    {
        url u { "http://www.google.com" };
        EXPECT_FALSE(!!u.path());
    }
    {
        url u { "http://www.google.com/" };
        ASSERT_TRUE(!!u.path());
        EXPECT_EQ("/", u.path().get());
    }
    {
        url u { "http://www.google.com:80/search" };
        ASSERT_TRUE(!!u.path());
        EXPECT_EQ("/search", u.path().get());
    }
    {
        url u { "http://www.google.com/search?q=url#fragment" };
        ASSERT_TRUE(!!u.path());
        EXPECT_EQ("/search", u.path().get());
    }
}

TEST(accessor, query)
{
    {
        url u { "http://www.google.com/search" };
        EXPECT_FALSE(!!u.query());
    }
    {
        url u { "http://www.google.com/search?q=url" };
        ASSERT_TRUE(!!u.query());
        EXPECT_EQ("q=url", u.query().get());
    }
    {
        url u { "http://www.google.com/search?q=url#fragment" };
        ASSERT_TRUE(!!u.query());
        EXPECT_EQ("q=url", u.query().get());
    }
}

TEST(accessor, fragment)
{
    {
        url u { "http://www.google.com/search" };
        EXPECT_FALSE(!!u.fragment());
    }
    {
        url u { "http://www.google.com/search#fragment" };
        ASSERT_TRUE(!!u.fragment());
        EXPECT_EQ("fragment", u.fragment().get());
    }
}

TEST(accessor, user_info)
{
    {
        url u { "http://www.google.com/search" };
        EXPECT_FALSE(!!u.user_info());
    }
    {
        url u { "http://john.doe:password@www.google.com/search#fragment" };
        ASSERT_TRUE(!!u.user_info());
        EXPECT_EQ("john.doe:password", u.user_info().get());
    }
}

TEST(accessor, at)
{
    url u { "http://www.google.com/search" };
    EXPECT_EQ('h', u.at(0));
    EXPECT_EQ('h', u.at(27));
    EXPECT_EQ('g', u[14]);
}
