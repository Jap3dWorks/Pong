//
// Created by Jordi on 8/2/2022.
//

#ifndef GL_TEST_REFLECTION_UTILS_H
#define GL_TEST_REFLECTION_UTILS_H

#include <iostream>

inline constexpr std::string class_name(const std::string& prettyFunction) {
    size_t colons = prettyFunction.find("::");
    if (colons == std::string::npos)
        return "::";
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;

    return prettyFunction.substr(begin,end);
}

#define CLASS_NAME_ class_name(__PRETTY_FUNCTION__)

#endif //GL_TEST_REFLECTION_UTILS_H
