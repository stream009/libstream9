#ifndef STREAM9_TYPE_TRAITS_HPP
#define STREAM9_TYPE_TRAITS_HPP

#include <type_traits>

namespace stream9 { namespace std14 {

template< class T >
using remove_cv_t = typename std::remove_cv<T>::type;

template< class T >
using remove_const_t = typename std::remove_const<T>::type;

template< class T >
using remove_volatile_t = typename std::remove_volatile<T>::type;

template< class T >
using add_cv_t = typename std::add_cv<T>::type;

template< class T >
using add_const_t = typename std::add_const<T>::type;

template< class T >
using add_volatile_t = typename std::add_volatile<T>::type;

template< class T >
using remove_reference_t = typename std::remove_reference<T>::type;

template< class T >
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

template< class T >
using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;

template< class T >
using remove_pointer_t = typename std::remove_pointer<T>::type;

template< class T >
using add_pointer_t = typename std::add_pointer<T>::type;

template< class T >
using make_signed_t = typename std::make_signed<T>::type;

template< class T >
using make_unsigned_t = typename std::make_unsigned<T>::type;

template< class T >
using remove_extent_t = typename std::remove_extent<T>::type;

template< class T >
using remove_all_extents_t = typename std::remove_all_extents<T>::type;

//template< std::size_t Len, std::size_t Align = /*default-alignment*/ >
//using aligned_storage_t = typename aligned_storage<Len, Align>::type;

//template< std::size_t Len, class... Types >
//using aligned_union_t = typename std::aligned_union<Len,Types...>::type;

template< class T >
using decay_t = typename std::decay<T>::type;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B,T>::type;

template< bool B, class T, class F >
using conditional_t = typename std::conditional<B,T,F>::type;

template< class... T >
using common_type_t = typename std::common_type<T...>::type;

template< class T >
using underlying_type_t = typename std::underlying_type<T>::type;

template< class T >
using result_of_t = typename std::result_of<T>::type;

} // namespace std14

// is_array_of
template<typename A, typename T>
struct is_array_of
    : std14::conditional_t<std::is_array<std14::remove_cv_t<A>>::value &&
                           std::is_same<T, std14::remove_all_extents_t<
                                               std14::remove_cv_t<A>
                                           >
                           >::value
        , std::true_type, std::false_type>
{};

// remove_cv_ref_t
template<typename T>
using remove_cv_ref_t =
    std14::remove_cv_t<std14::remove_reference_t<T>>;

// is_any_of
template<typename T, typename... Args>
struct is_any_of : std::false_type
{};

template<typename T, typename Head, typename... Rest>
struct is_any_of<T, Head, Rest...>
    : std14::conditional_t<
            std::is_same<T, Head>::value, std::true_type,
                                          is_any_of<T, Rest...>
      >
{};

template<typename T, typename Head>
struct is_any_of<T, Head> : std::is_same<T, Head>
{};

// is_specialization_of

namespace detail {

template<typename T, template<typename...> class Template>
    struct is_specialization_of : std::false_type {};

template<template<typename...> class Template, typename... Args>
    struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

} // namespace detail

template<typename T, template<typename...> class Template>
using is_specialization_of =
            detail::is_specialization_of<std14::decay_t<T>, Template>;

// is_iterator<It, ItratorCategory>
template<typename It, typename Category = std::input_iterator_tag>
using is_iterator =
    std::is_base_of<
        Category,
        typename std::iterator_traits<It>::iterator_category>;

} // namespace stream9

#endif // STREAM9_TYPE_TRAITS_HPP
