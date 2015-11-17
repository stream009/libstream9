#ifndef IOSTREAM_QUOTED_HPP
#define IOSTREAM_QUOTED_HPP

#include <sstream>
#include <string>
#include <utility>

namespace ios {

template<typename Str>
std::string delimited(char open, const Str &str)
{
    std::ostringstream oss;

    char close = open;
    if (open == '(') {
        close = ')';
    }
    else if (open == '[') {
        close = ']';
    }
    else if (open == '{') {
        close = '}';
    }
    else if (open == '<') {
        close = '>';
    }
    else if (open == '`') {
        close ='\'';
    }

    //TODO escape

    oss << open << str << close;
    return oss.str();
}

template<typename Str>
std::string
quoted(Str &&str)
{
    return delimited('"', std::forward<Str>(str));
}

} // namespace ios

#endif // IOSTREAM_QUOTED_HPP
