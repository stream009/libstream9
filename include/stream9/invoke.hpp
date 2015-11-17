#ifndef STREAM9_INVOKE_HPP
#define STREAM9_INVOKE_HPP

#if __cplusplus <= 201402L

// Implementation of std::invoke on C++17
// Borrowed from http://en.cppreference.com/w/cpp/utility/functional/invoke

#include <utility>
#include <type_traits>

namespace stream9 {

namespace detail {

template <class F, class... Args>
inline auto INVOKE(F&& f, Args&&... args) ->
    decltype(std::forward<F>(f)(std::forward<Args>(args)...)) {
      return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <class Base, class T, class Derived>
inline auto INVOKE(T Base::*pmd, Derived&& ref) ->
    decltype(std::forward<Derived>(ref).*pmd) {
      return std::forward<Derived>(ref).*pmd;
}

template <class PMD, class Pointer>
inline auto INVOKE(PMD pmd, Pointer&& ptr) ->
    decltype((*std::forward<Pointer>(ptr)).*pmd) {
      return (*std::forward<Pointer>(ptr)).*pmd;
}

template <class Base, class T, class Derived, class... Args>
inline auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args) ->
    decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)) {
      return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
}

template <class PMF, class Pointer, class... Args>
inline auto INVOKE(PMF pmf, Pointer&& ptr, Args&&... args) ->
    decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)) {
      return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
}

} // namespace detail

template< class F, class... ArgTypes>
auto invoke(F&& f, ArgTypes&&... args) ->
  decltype(detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...)) {
    return detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
}

} // namespace stream9

#else // C++17

#include <functional>

#endif // if __cplusplus <= 201402L

#endif // STREAM9_INVOKE_HPP
