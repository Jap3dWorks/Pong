//
// Created by Jordi on 8/16/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_SERIAL_TYPES_H_
#define PONG_SRC_PONG_SERIALIZER_SERIAL_TYPES_H_
#include <optional>
#include <iostream>

namespace pong::serializer {
    using Version=std::optional<uint32_t>;

    using DataSize = size_t;

    struct Any_t{};
    struct reference_data_t{};

}


#endif //PONG_SRC_PONG_SERIALIZER_SERIAL_TYPES_H_
