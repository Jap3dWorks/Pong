//
// Created by Jordi on 8/23/2022.
//

#ifndef PONG_BASIC_TESTS_H
#define PONG_BASIC_TESTS_H

#include "Utils/logger.h"
#include "Pong/serial_data/header_data.h"
#include <vector>
#include <optional>
#include <iostream>
#include <string>


void test_len(const char value[]) {
    LOG_INFO("Len "<< (sizeof(value) / sizeof(char)) );
}

template<typename T>
struct TestTemp {
};

template<>
struct TestTemp<const char [24]> {
    using type = const char [24];
    static inline void func(type &value) {
        LOG_INFO("char[0]: " << value[0]);
        LOG_INFO("char*: " << (char*)&value);
    }
};


void test_char_list_cpy() {
    const char namer[24]{};
    const char* value = "value";

    LOG_INFO("String len: " << strlen(value));

    strncpy((char*)namer, value, strlen(value));

    LOG_INFO("String namer: " << namer);

    LOG_INFO("char[] len: " << strlen((const char*)namer));

    auto str = std::string(namer);
    LOG_INFO("as str: " << str);

    test_len(namer);

    TestTemp<decltype(namer)>::func(namer);

}




#endif //PONG_BASIC_TESTS_H
