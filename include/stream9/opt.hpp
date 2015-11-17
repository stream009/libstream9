#ifndef STREAM9_OPT_HPP
#define STREAM9_OPT_HPP

#include <boost/optional.hpp>

#include <stream9/type_traits.hpp>

namespace stream9 {

template<typename T>
class opt : public boost::optional<T>
{
    using base_t = boost::optional<T>;
public:
    opt() = default;

    opt(boost::none_t) : base_t { boost::none } {}

    // forward to copy/move constructor
    template<typename Arg,
        typename std14::enable_if_t<
            std::is_same<base_t, std14::decay_t<Arg>>::value
        >* = nullptr
    >
    opt(Arg&& v) : base_t { std::forward<Arg>(v) } {}

    // forward to optional(bool, T) constructor
    template<typename Arg,
        typename std14::enable_if_t<
            std::is_same<std14::decay_t<T>, std14::decay_t<Arg>>::value
        >* = nullptr
    >
    opt(bool condition, Arg&& v) : base_t { condition, std::forward<Arg>(v) } {}

    // otherwise forward arguments to T's constructor
    template<typename... Args>
    opt(Args&&... args)
        : base_t { T (std::forward<Args>(args)...) } {}
};

} // namespace stream9

#endif // STREAM9_OPT_HPP
