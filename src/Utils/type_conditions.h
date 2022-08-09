//
// Created by Jordi on 7/24/2022.
//

#ifndef GL_TEST_TYPE_CONDITIONS_H
#define GL_TEST_TYPE_CONDITIONS_H
#include <iostream>
#include <type_traits>
#include <utility>


//template<typename T>
//struct empty{};
//
//template<typename... Ts>
//struct unique;
//
//template<>
//struct unique<>
//{
//    static constexpr bool value = true;
//};
//
//template<typename T, typename... Ts>
//struct unique<T, Ts...>: public unique<Ts...>, public empty<T>
//{
//    using base = unique<Ts...>;
//
//    static constexpr bool value = base::value and not std::is_base_of<empty<T>, base>::value;
//};


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


#endif //GL_TEST_TYPE_CONDITIONS_H
