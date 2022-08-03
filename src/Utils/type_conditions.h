//
// Created by Jordi on 7/24/2022.
//

#ifndef GL_TEST_TYPE_CONDITIONS_H
#define GL_TEST_TYPE_CONDITIONS_H
#include <iostream>
#include <concepts>

//#define BASE_CONDITION_GEN(_base) \

template<class T, class U> concept CondIsBase = std::is_base_of<U, T>::value;

#define ENSURE_LVALUE(T) typename std::enable_if<!std::is_lvalue_reference<T>::value>::type


#endif //GL_TEST_TYPE_CONDITIONS_H
