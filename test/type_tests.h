//
// Created by Jordi on 9/3/2022.
//

#ifndef PONG_TEST_TYPE_TESTS_H_
#define PONG_TEST_TYPE_TESTS_H_

#include "Utils/logger.h"

#include <iostream>
#include <type_traits>

namespace type_tests {

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
}

#endif //PONG_TEST_TYPE_TESTS_H_
