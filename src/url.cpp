#include <cassert>
#include <ostream>

#include <boost/utility/string_ref.hpp>

#include <http_parser.h>

#include <stream9/url.hpp>

namespace stream9 {

using value_type = url::value_type;
using size_type = url::size_type;
using difference_type = url::difference_type;
using const_reference = url::const_reference;
using const_pointer = url::const_pointer;
using const_iterator = url::const_iterator;
using const_reverse_iterator = url::const_reverse_iterator;

url::
url(const char* const c_str)
    : m_string { c_str }
{
    parse();
}

url::
url(const char* const c_str, const size_t len)
    : m_string { c_str, len }
{
    parse();
}

url::
url(const std::string &str)
    : m_string { str }
{
    parse();
}

url::
url(std::string &&str)
    : m_string { std::move(str) }
{
    parse();
}

url::
url(const boost::string_ref &ref)
    : m_string { ref.begin(), ref.end() }
{
    parse();
}

boost::string_ref url::
scheme() const
{
    return { m_string.data() + m_scheme.offset, m_scheme.length };
}

boost::optional<boost::string_ref> url::
host() const
{
    if (m_host.empty()) {
        return boost::none;
    }
    else {
        return boost::string_ref {
            m_string.data() + m_host.offset, m_host.length };
    }
}

boost::optional<uint16_t> url::
port() const
{
    if (m_port) {
        return m_port;
    }
    else {
        return boost::none;
    }
}

boost::optional<boost::string_ref> url::
path() const
{
    if (m_path.empty()) {
        return boost::none;
    }
    else {
        return boost::string_ref {
                m_string.data() + m_path.offset, m_path.length };
    }
}

boost::optional<boost::string_ref> url::
query() const
{
    if (m_query.empty()) {
        return boost::none;
    }
    else {
        return boost::string_ref {
                m_string.data() + m_query.offset, m_query.length };
    }
}

boost::optional<boost::string_ref> url::
fragment() const
{
    if (m_fragment.empty()) {
        return boost::none;
    }
    else {
        return boost::string_ref {
                m_string.data() + m_fragment.offset, m_fragment.length };
    }
}

boost::optional<boost::string_ref> url::
user_info() const
{
    if (m_user_info.empty()) {
        return boost::none;
    }
    else {
        return boost::string_ref {
                m_string.data() + m_user_info.offset, m_user_info.length };
    }
}

const_reference url::
at(const size_type pos) const { return m_string.at(pos); }

void url::
scheme(const boost::string_ref &scheme)
{
    rebuild(scheme, host(), port(), path(), query(), fragment(), user_info());
}

void url::
host(const stream9::opt<boost::string_ref> &host)
{
    rebuild(scheme(), host, port(), path(), query(),
                                    fragment(), user_info());
}

void url::
port(const stream9::opt<uint16_t> port)
{
    rebuild(scheme(), host(), port,
            path(), query(), fragment(), user_info());
}

void url::
path(const stream9::opt<boost::string_ref> &path)
{
    rebuild(scheme(), host(), port(), path, query(),
                                      fragment(), user_info());
}

void url::
query(const stream9::opt<boost::string_ref> &query)
{
    rebuild(scheme(), host(), port(), path(), query,
                                      fragment(), user_info());
}

void url::
fragment(const stream9::opt<boost::string_ref> &fragment)
{
    rebuild(scheme(), host(), port(), path(), query(),
                                      fragment, user_info());
}

void url::
user_info(const stream9::opt<boost::string_ref> &user_info)
{
    rebuild(scheme(), host(), port(), path(), query(),
                                      fragment(), user_info);
}

bool url::
has_authority() const
{
    return !!host();
}

size_type url::
size() const { return m_string.size(); }

const_reference url::
front() const { return m_string.front(); }

const_reference url::
back() const { return m_string.back(); }

const_iterator url::
begin() const { return m_string.begin(); }

const_iterator url::
cbegin() const { return m_string.cbegin(); }

const_reverse_iterator url::
rbegin() const { return m_string.rbegin(); }

const_reverse_iterator url::
crbegin() const { return m_string.crbegin(); }

const_iterator url::
end() const { return m_string.end(); }

const_iterator url::
cend() const { return m_string.cend(); }

const_reverse_iterator url::
rend() const { return m_string.rend(); }

const_reverse_iterator url::
crend() const { return m_string.crend(); }

const_pointer url::
c_str() const { return m_string.c_str(); }

const_reference url::
operator[](const size_type pos) const { return m_string[pos]; }

bool url::
operator==(const url &rhs) const { return m_string == rhs.m_string; }

bool url::
operator==(const std::string &rhs) const { return m_string == rhs; }

bool url::
operator==(const boost::string_ref &rhs) const { return m_string == rhs; }

bool url::
operator==(const char* const rhs) const { return m_string == rhs; }

bool url::
operator<(const url &rhs) const { return m_string < rhs.m_string; }

bool url::
operator<(const std::string &rhs) const { return m_string < rhs; }

bool url::
operator<(const boost::string_ref &rhs) const { return m_string < rhs; }

bool url::
operator<(const char* const rhs) const { return m_string < rhs; }

void url::
swap(url &rhs)
{
    m_string.swap(rhs.m_string);

    m_scheme.swap(rhs.m_scheme);
    m_host.swap(rhs.m_host);
    m_path.swap(rhs.m_path);
    m_query.swap(rhs.m_query);
    m_fragment.swap(rhs.m_fragment);
    m_user_info.swap(rhs.m_user_info);

    std::swap(m_port, rhs.m_port);
}

size_t url::
hash() const
{
    std::hash<std::string> hash;
    return hash(m_string);
}

void url::
parse()
{
    ::http_parser_url url;

    const auto rc =::http_parser_parse_url(
                            m_string.c_str(), m_string.size(), 0, &url);
    if (rc != 0) {
        throw parse_error { m_string };
    }

    m_port = url.port;
    for (auto i = 0u; i < UF_MAX; ++i) {
        if ((url.field_set & (1 << i)) == 0) continue;

        const auto &data = url.field_data[i];
        switch (i) {
        case UF_SCHEMA:
            m_scheme.offset = data.off;
            m_scheme.length = data.len;
            break;
        case UF_HOST:
            m_host.offset = data.off;
            m_host.length = data.len;
            break;
        case UF_PORT:
            break;
        case UF_PATH:
            m_path.offset = data.off;
            m_path.length = data.len;
            break;
        case UF_QUERY:
            m_query.offset = data.off;
            m_query.length = data.len;
            break;
        case UF_FRAGMENT:
            m_fragment.offset = data.off;
            m_fragment.length = data.len;
            break;
        case UF_USERINFO:
            m_user_info.offset = data.off;
            m_user_info.length = data.len;
            break;
        default:
            assert(false);
        }
    }

    validate();
}

void url::
rebuild(const boost::string_ref &scheme,
        const boost::optional<boost::string_ref> &host,
        const boost::optional<uint16_t> &port,
        const boost::optional<boost::string_ref> &path,
        const boost::optional<boost::string_ref> &query,
        const boost::optional<boost::string_ref> &fragment,
        const boost::optional<boost::string_ref> &user_info)
{
    std::string str;

    str.append(scheme.data(), scheme.size());
    if (has_authority()) {
        str.append("://");

        if (user_info) {
            str.append(user_info->data(), user_info->size());
            str.append("@");
        }

        str.append(host->data(), host->size());

        if (port) {
            str.append(":");
            str.append(std::to_string(*port));
        }
    }
    else {
        str.append(":");
    }

    if (path) {
        str.append(path->data(), path->size());
    }

    if (query) {
        str.append("?");
        str.append(query->data(), query->size());
    }

    if (fragment) {
        str.append("#");
        str.append(fragment->data(), fragment->size());
    }

    url new_url { std::move(str) };
    swap(new_url);
}

bool url::field::
empty() const
{
    return offset == 0 && length == 0;
}

void url::field::
swap(field &rhs)
{
    std::swap(offset, rhs.offset);
    std::swap(length, rhs.length);
}

// url::parse_error

url::parse_error::
parse_error(const std::string &url) : m_url { url } {}

const char* url::parse_error::
what() const noexcept
{
    return m_url.c_str();
}

// free functions

std::ostream &
operator<<(std::ostream &os, const url &u)
{
    return os << u.c_str();
}

std::string to_string(const url &url)
{
    return { url.begin(), url.end() };
}

namespace literal {

url operator""_u(const char* str, size_t len)
{
    return { str, len };
}

} // namespace literal

} // namespace stream9
