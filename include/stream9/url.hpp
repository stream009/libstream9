#ifndef STREAM9_URL_HPP
#define STREAM9_URL_HPP

#include <cassert>
#include <iosfwd>
#include <string>

#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>
#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/range/empty.hpp>
#include <boost/utility/string_ref.hpp>

#include <stream9/opt.hpp>

namespace stream9 {

class url
    : boost::equality_comparable<url
    , boost::equality_comparable<url, std::string
    , boost::equality_comparable<url, boost::string_ref
    , boost::equality_comparable<url, const char*
    , boost::less_than_comparable<url
    , boost::less_than_comparable<url, std::string
    , boost::less_than_comparable<url, boost::string_ref
    , boost::less_than_comparable<url, const char*
      > > > > > > > >
{
public:
    using value_type = std::string::value_type;
    using size_type = std::string::size_type;
    using difference_type = std::string::difference_type;
    using const_reference = std::string::const_reference;
    using reference = const_reference;
    using const_pointer = std::string::const_pointer;
    using pointer = const_pointer;
    using const_iterator = std::string::const_iterator;
    using iterator = const_iterator;
    using const_reverse_iterator = std::string::const_reverse_iterator;
    using reverse_iterator = const_reverse_iterator;

    class parse_error;

public:
    url(const value_type* c_str);
    url(const value_type* c_str, const size_type len);

    url(const std::string &str);
    url(std::string &&str);

    url(const boost::string_ref &str);

    template<typename It,
        BOOST_CONCEPT_REQUIRES(
            ((boost::ForwardIterator<It>)),
            (void*)
        ) = nullptr>
    url(It begin, It end)
        : m_string { begin, end }
    {
        parse();
    }

    url(const url &rhs) = default;
    url(url&&) = default;

    url &operator=(const url&) = default;
    url &operator=(url&&) = default;

    // accessor
    boost::string_ref                  scheme() const;
    boost::optional<boost::string_ref> host() const;
    boost::optional<uint16_t>          port() const;
    boost::optional<boost::string_ref> path() const;
    boost::optional<boost::string_ref> query() const;
    boost::optional<boost::string_ref> fragment() const;
    boost::optional<boost::string_ref> user_info() const;

    const_reference at(size_type pos) const;

    // modifier
    void scheme(const boost::string_ref&);
    void host(const stream9::opt<boost::string_ref>&);
    void port(const stream9::opt<uint16_t> port);
    void path(const stream9::opt<boost::string_ref>&);
    void query(const stream9::opt<boost::string_ref>&);
    void fragment(const stream9::opt<boost::string_ref>&);
    void user_info(const stream9::opt<boost::string_ref>&);

    // query
    bool has_authority() const;
    size_type size() const;

    const_reference front() const;
    const_reference back() const;

    const_iterator         begin() const;
    const_iterator         cbegin() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    const_iterator         end() const;
    const_iterator         cend() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    const_pointer     c_str() const;

    // operator
    const_reference operator[](size_type pos) const;

    bool operator==(const url&) const;
    bool operator==(const std::string&) const;
    bool operator==(const boost::string_ref&) const;
    bool operator==(const char*) const;

    bool operator<(const url&) const;
    bool operator<(const std::string&) const;
    bool operator<(const boost::string_ref&) const;
    bool operator<(const char*) const;

    void swap(url &rhs);
    size_t hash() const;

private:
    struct field {
        uint16_t offset = 0;
        uint16_t length = 0;

        bool empty() const;
        void swap(field &rhs);
    };

    void parse();

    void rebuild(const boost::string_ref &scheme,
                 const boost::optional<boost::string_ref> &host,
                 const boost::optional<uint16_t> &port,
                 const boost::optional<boost::string_ref> &path,
                 const boost::optional<boost::string_ref> &query,
                 const boost::optional<boost::string_ref> &fragment,
                 const boost::optional<boost::string_ref> &user_info);

    friend std::ostream &operator<<(std::ostream&, const url&);

    void validate() const
    {
        assert(!m_scheme.empty());
        assert(!m_host.empty());
    }

private:
    std::string m_string;

    field m_scheme;
    field m_host;
    field m_path;
    field m_query;
    field m_fragment;
    field m_user_info;
    uint16_t m_port = 0;
};

class url::parse_error : public std::exception
{
public:
    parse_error(const std::string&);
    const char*what() const noexcept;
private:
    std::string m_url;
};

std::string to_string(const url&);

namespace literal {

url operator""_u(const char* str, size_t len);

} // namespace literal

} // namespace stream9

namespace std {

template<>
inline void swap(stream9::url &a, stream9::url &b)
{
    a.swap(b);
}

template<>
struct hash<stream9::url>
{
    size_t operator()(const stream9::url &u) const
    {
        return u.hash();
    }
};

} // namespace std

#endif // STREAM9_URL_HPP
