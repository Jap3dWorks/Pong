//
// Created by Jordi on 9/3/2022.
//

#ifndef PONG_TEST_TYPE_TESTS_H_
#define PONG_TEST_TYPE_TESTS_H_

#include "Utils/logger.h"
#include "Utils/hash_utils.h"

#include <iostream>
#include <type_traits>


template<size_t N>
struct TestInt: std::integral_constant<size_t, N> {};


void test_integral_constant() {
    static_assert(std::integral_constant<size_t, 0>()==0, "Incorrect number");
    constexpr auto a = std::integral_constant<size_t, 0>();
    constexpr size_t b = std::integral_constant<size_t, 0>();

    LOG_INFO("Type: " << typeid(a).name());
    LOG_INFO("Type: " << typeid(b).name());

    LOG_INFO(" " << a);
    LOG_INFO(" " << a + 1);
    LOG_INFO(" " << a() + 1);
    LOG_INFO(" " << b);
    LOG_INFO(" " << b);
}

void test_compiletime_hash() {
    LOG_INFO("test_compiletime_hash:");
    using hash_type = TestInt<COMPILE_TIME_CRC32_STR("RelationComp")>;

    LOG_INFO(hash_type::value);
};


void type_tests() {
    test_compiletime_hash();
}

#endif //PONG_TEST_TYPE_TESTS_H_
