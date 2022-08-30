//
// Created by Jordi on 7/24/2022.
//

#ifndef GL_TEST_TYPE_CONDITIONS_H
#define GL_TEST_TYPE_CONDITIONS_H
#include <iostream>
#include <type_traits>
#include <utility>
#include <optional>


template <typename... Ts>
struct is_intersection;

template<typename T>
struct is_intersection<T>
{
    static constexpr bool value = false;
};

template<typename T, typename U, typename... Ts>
struct is_intersection<T, U, Ts...> {
    static constexpr bool value = std::is_same<T, U>::value or
        is_intersection<T, Ts...>::value;
    ;
};


#define ENSURE_LVALUE(T) typename std::enable_if<!std::is_lvalue_reference<T>::value>::type

template<typename T, typename ...Ts>
concept Intersects = is_intersection<T, Ts...>::value;


template<typename T, typename U, U  value_, typename V, V ...values_>
struct op_value: op_value<T, U, value_, bool>, op_value<T, V, values_...>
{};

template<typename T, T value_>
struct op_value<T, T, value_, std::nullopt_t> {
    using type=T;
    static constexpr T value{value_};
    static constexpr bool has_value{true};
};

template<typename T>
struct op_value<T, std::nullopt_t, std::nullopt, std::nullopt_t> {
    using type=T;
    static constexpr bool has_value{false};
};


/**
 * Get the class at index N in a variadic argument template.
 * @tparam N
 * @tparam T
 * @tparam Args
 */
template<uint32_t N, typename T, typename ...Args>
struct variadic_id_t: variadic_id_t<N-1, Args...>{
};

template<typename T, typename ...Args>
struct variadic_id_t<0, T, Args...> {
    static constexpr uint32_t index{0};
    using type = T;
};


template<typename ...Args>
struct TypesStruct  {

    static inline constexpr uint32_t count{sizeof...(Args)};

    template<uint32_t N>
    using get = variadic_id_t<N, Args...>;
};


template<typename T>
struct BaseTemplate: public std::false_type {};

template<template<typename ...Args> typename T, typename ...Args>
struct BaseTemplate<T<Args...>> : public std::true_type {

    template<typename ...args_>
    using TemplateType = T<args_...>;

    using Types = TypesStruct<Args...>;
};


template<template<typename ...Args> typename T, typename U>
struct IsTemplatedFrom: public std::false_type {};

template<template<typename ...Args> typename T, typename U>
struct IsTemplatedFrom<T, T<U>>: public std::true_type {};


template<typename U, template<typename ...Args> typename T>
concept templated_from = IsTemplatedFrom<T, U>::value;


#endif //GL_TEST_TYPE_CONDITIONS_H
