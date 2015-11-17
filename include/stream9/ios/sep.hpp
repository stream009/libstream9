#ifndef IOSTREAM_SEP_HPP
#define IOSTREAM_SEP_HPP

#include <cassert>
#include <memory>
#include <ostream>
#include <streambuf>
#include <locale>
#include <string>

#include <boost/make_unique.hpp> // until C++11

namespace ios {

namespace detail {

template<typename CharT>
constexpr const CharT *default_sep();

template<>
inline constexpr const char*
default_sep<>() __attribute__ ((unused));

template<>
inline constexpr const char*
default_sep<>()
{
    return " ";
}

template<>
inline constexpr const wchar_t*
default_sep<>() __attribute__ ((unused));

template<>
inline constexpr const wchar_t*
default_sep<>()
{
    return L" ";
}


template<typename CharT>
class basic_sep
{
public:
    template<typename Str>
    basic_sep(Str &&str, bool put_newline_at_end = true)
        : m_str { std::move(str) }
        , m_put_newline_at_end { put_newline_at_end }
    {}

    basic_sep() = default;

    // query
    const CharT *c_str() const { return m_str.c_str(); }
    size_t size() const { return m_str.size(); }

    bool put_newline_at_end() const { return m_put_newline_at_end; }

private:
    std::basic_string<CharT> m_str { default_sep<CharT>() };
    bool m_put_newline_at_end = true;
};


class nosep {};


template<typename CharT, typename Traits = std::char_traits<CharT>>
class basic_sep_ostreambuf : public std::basic_streambuf<CharT, Traits>
{
    using Base = std::basic_streambuf<CharT, Traits>;
public:
    using char_type = typename Base::char_type;
    using int_type = typename Base::int_type;
    using pos_type = typename Base::pos_type;
    using off_type = typename Base::off_type;
    using traits_type = typename Base::traits_type;

public:
    basic_sep_ostreambuf(
            std::streambuf &buf, basic_sep<CharT> &&sep, const std::locale locale)
        : m_buf { buf }
        , m_sep { std::move(sep) }
        , m_locale { locale }
    {}

    ~basic_sep_ostreambuf() override
    {
        if (m_sep.put_newline_at_end()) {
            const auto &ctype = std::use_facet<std::ctype<CharT>>(m_locale);
            this->sputc(ctype.widen('\n'));
            this->pubsync();
        }
    }

    basic_sep<CharT> &sep() { return m_sep; }
    std::basic_streambuf<CharT, Traits> &base() { return m_buf; }

protected:
    void imbue(const std::locale &loc) override
    {
        m_buf.pubimbue(loc);
    }

    std::streambuf *setbuf(char_type *s, std::streamsize n) override
    {
        return m_buf.pubsetbuf(s, n);
    }

    pos_type seekoff(off_type off,
                     std::ios_base::seekdir dir,
                     std::ios_base::openmode which
                            = std::ios_base::in | std::ios_base::out) override
    {
        return m_buf.pubseekoff(off, dir, which);
    }

    int sync() override
    {
        return m_buf.pubsync();
    }

    std::streamsize xsputn(const char_type* s, std::streamsize count) override
    {
        if (m_first) {
            m_first = false;
        }
        else {
            m_buf.sputn(m_sep.c_str(), m_sep.size());
        }

        return m_buf.sputn(s, count);
    }

    int_type overflow(int_type ch = traits_type::eof()) override
    {
        return m_buf.sputc(ch);
    }

private:
    std::basic_streambuf<CharT, Traits> &m_buf;
    basic_sep<CharT> m_sep;
    std::locale m_locale;
    bool m_first = true;
};


template<typename CharT, typename Traits = std::char_traits<CharT>>
class basic_sep_ostream : public std::basic_ostream<CharT, Traits>
{
    using StreamBuf =  basic_sep_ostreambuf<CharT, Traits>;
public:
    basic_sep_ostream(std::basic_ostream<CharT, Traits> &os,
                      basic_sep<CharT> &&sep)
        : m_buf { boost::make_unique<StreamBuf>(*os.rdbuf(),
                                                std::move(sep), os.getloc()) }
    {
        assert(os.rdbuf());
        this->rdbuf(m_buf.get());
    }

    basic_sep_ostream(basic_sep_ostream &&rhs)
        : m_buf { std::move(rhs.m_buf) }
    {
        this->rdbuf(m_buf.get());
    }

private:
    std::unique_ptr<StreamBuf> m_buf;
};

template<typename CharT, typename Traits = std::char_traits<CharT>>
basic_sep_ostream<CharT, Traits>
    operator<<(std::basic_ostream<CharT, Traits> &os,
                                        basic_sep<CharT> &&sep)
{
    return basic_sep_ostream<CharT, Traits> { os, std::move(sep) };
}


template<typename CharT, typename Traits = std::char_traits<CharT>>
class basic_nosep_ostream : public std::basic_ostream<CharT, Traits>
{
public:
    basic_nosep_ostream(std::basic_ostream<CharT, Traits> &os)
    {
        auto* const buf =
             dynamic_cast<basic_sep_ostreambuf<CharT, Traits>*>(os.rdbuf());
        assert(buf);

        this->rdbuf(&buf->base());
    }

    basic_nosep_ostream(basic_nosep_ostream &&) {}
};

template<typename CharT, typename Traits = std::char_traits<CharT>>
basic_nosep_ostream<CharT, Traits>
    operator<<(std::basic_ostream<CharT, Traits> &os, const nosep&)
{
    return basic_nosep_ostream<CharT, Traits> { os };
}

} // namespace detail


using sep = detail::basic_sep<char>;
using wsep = detail::basic_sep<wchar_t>;
using nosep = detail::nosep;

} // namespace ios

#endif //IOSTREAM_SEP_HPP
