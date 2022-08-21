//
// Created by Jordi on 8/16/2022.
//

#ifndef GL_TEST_SERIALIZE_TYPES_H
#define GL_TEST_SERIALIZE_TYPES_H
#include <optional>
#include <iostream>

namespace Pong::serializer {
    using Version=std::optional<uint32_t>;

    using data_size_t = size_t;

    struct Any_t{};
    struct reference_data_t{};

}


#endif //GL_TEST_SERIALIZE_TYPES_H
