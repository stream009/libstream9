#ifndef STREAM9_LITERALS_HPP
#define STREAM9_LITERALS_HPP

#include <chrono>

namespace stream9 { namespace std14 {

inline namespace literals {

inline namespace chrono_literals {

constexpr std::chrono::hours
operator ""_h(unsigned long long h)
{
    return std::chrono::hours(h);
}

constexpr std::chrono::duration<long double, std::ratio<3600,1>>
operator ""_h(long double h)
{
    return std::chrono::duration<long double, std::ratio<3600,1>>(h);
}

constexpr std::chrono::minutes
operator ""_min(unsigned long long m)
{
    return std::chrono::minutes(m);
}

constexpr std::chrono::duration<long double, std::ratio<60,1>>
operator ""_min(long double m)
{
    return std::chrono::duration<long double, std::ratio<60,1>> (m);
}

constexpr std::chrono::seconds
operator ""_s(unsigned long long s)
{
    return std::chrono::seconds(s);
}

constexpr std::chrono::duration<long double>
operator ""_s(long double s)
{
    return std::chrono::duration<long double>(s);
}

constexpr std::chrono::milliseconds
operator ""_ms(unsigned long long ms)
{
    return std::chrono::milliseconds(ms);
}

constexpr std::chrono::duration<long double, std::milli>
operator ""_ms(long double ms)
{
    return std::chrono::duration<long double, std::milli>(ms);
}

constexpr std::chrono::microseconds
operator ""_us(unsigned long long us)
{
    return std::chrono::microseconds(us);
}

constexpr std::chrono::duration<long double, std::micro>
operator ""_us(long double us)
{
    return std::chrono::duration<long double, std::micro>(us);
}

constexpr std::chrono::nanoseconds
operator ""_ns(unsigned long long ns)
{
    return std::chrono::nanoseconds(ns);
}

constexpr std::chrono::duration<long double, std::nano>
operator ""_ns(long double ns)
{
    return std::chrono::duration<long double, std::nano>(ns);
}

} // namespace chrono_literals

} // namespace literals

}} // namespace stream9::std14

#endif // STREAM9_LITERALS_HPP
