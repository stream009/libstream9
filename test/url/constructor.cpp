#include <stream9/url.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/utility/string_ref.hpp>

#include <gtest/gtest.h>

using url = stream9::url;

TEST(constructor, from_c_str)
{
    ASSERT_NO_THROW({
        url u { "http://www.google.com:8080" };
    });
}

TEST(constructor, from_c_str_with_length)
{
    ASSERT_NO_THROW({
        url u("http://www.google.com:8080", 21);
        EXPECT_STREQ("http://www.google.com", u.c_str());
    });

}

TEST(constructor, from_string)
{
    // l-value
    ASSERT_NO_THROW({
        std::string str { "http://www.google.com" };
        url u(str);
    });

    // const l-value
    ASSERT_NO_THROW({
        const std::string str { "http://www.google.com" };
        url u(str);
    });

    // r-value
    ASSERT_NO_THROW({
        std::string str { "http://www.google.com" };
        url u(std::move(str));
    });

    // const r-value
    ASSERT_NO_THROW({
        const std::string str { "http://www.google.com" };
        url u(std::move(str));
    });
}
#if 0
TEST(constructor, from_string_ref)
{
    // l-value
    ASSERT_NO_THROW({
        boost::string_ref ref { "http://www.google.com" };
        url u { ref };
    });

    // const l-value
    ASSERT_NO_THROW({
        const boost::string_ref ref { "http://www.google.com" };
        url u { ref };
    });

    // r-value
    ASSERT_NO_THROW({
        boost::string_ref ref { "http://www.google.com" };
        url u { std::move(ref) };
    });

    // const r-value
    ASSERT_NO_THROW({
        const boost::string_ref ref { "http://www.google.com" };
        url u { std::move(ref) };
    });
}
#endif
#if 0
TEST(constructor, from_iterator_range)
{
    const char str[] = "http://www.google.com";
    const char* const begin = str;
    const char* const end = str + sizeof(str) - 1;

    // l-value
    ASSERT_NO_THROW({
        boost::iterator_range<const char*> ref (begin, end);
        url u { ref };
    });

    // const l-value
    ASSERT_NO_THROW({
        const boost::iterator_range<const char*> ref (begin, end);
        url u { ref };
    });

    // r-value
    ASSERT_NO_THROW({
        boost::iterator_range<const char*> ref (begin, end);
        url u { std::move(ref) };
    });

    // const r-value
    ASSERT_NO_THROW({
        const boost::iterator_range<const char*> ref (begin, end);
        url u { std::move(ref) };
    });
}
#endif
TEST(constructor, from_iterator)
{
    ASSERT_NO_THROW({
        std::string str { "http://www.google.com" };
        url u (str.begin(), str.end());
    });

    ASSERT_NO_THROW({
        const std::string str { "http://www.google.com" };
        url u (str.begin(), str.end());
    });
}

TEST(constructor, copy_move)
{
    // l-value
    ASSERT_NO_THROW({
        url from { "http://www.google.com" };
        url to { from };

        EXPECT_STREQ(from.c_str(), to.c_str());
    });

    // const l-value
    ASSERT_NO_THROW({
        const url from { "http://www.google.com" };
        url to { from };

        EXPECT_STREQ(from.c_str(), to.c_str());
    });

    // r-value
    ASSERT_NO_THROW({
        url from { "http://www.google.com" };
        url to { std::move(from) };

        EXPECT_STREQ("http://www.google.com", to.c_str());
    });

    // const r-value
    ASSERT_NO_THROW({
        const url from { "http://www.google.com" };
        url to { std::move(from) };

        EXPECT_STREQ(from.c_str(), to.c_str());
    });
}

TEST(constructor, assignment)
{
    // l-value
    ASSERT_NO_THROW({
        url from { "http://www.google.com" };
        url to = from;

        EXPECT_STREQ(from.c_str(), to.c_str());
    });

    // const l-value
    ASSERT_NO_THROW({
        url from { "http://www.google.com" };
        const url to = from;

        EXPECT_STREQ(from.c_str(), to.c_str());
    });

    // r-value
    ASSERT_NO_THROW({
        url from { "http://www.google.com" };
        url to = std::move(from);

        EXPECT_STREQ("http://www.google.com", to.c_str());
    });

    // const r-value
    ASSERT_NO_THROW({
        const url from { "http://www.google.com" };
        url to = std::move(from);

        EXPECT_STREQ(from.c_str(), to.c_str());
    });

    // copy assign std::string
    ASSERT_NO_THROW({
        const std::string from { "http://www.google.com" };
        url to = from;

        EXPECT_STREQ(from.c_str(), to.c_str());
    });

    // move assign std::string
    ASSERT_NO_THROW({
        std::string from { "http://www.google.com" };
        url to = std::move(from);

        EXPECT_STREQ("http://www.google.com", to.c_str());
    });
}
