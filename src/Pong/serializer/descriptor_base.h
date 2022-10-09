//
// Created by Jordi on 10/8/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_BASE_H_
#define PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_BASE_H_

#include <iostream>
#include <vector>

#include "Pong/serializer/serial_types.h"
#include "Pong/serializer/header_data.h"


namespace pong::serializer {

    template<typename T>
    using HeadedDataT = HeadedData<Header<T>, T>;

    template<typename T>
    using SerializeDataT = HeadedDataT<std::vector<HeadedDataT<T> > >;

    class BaseDescriptor {};

    template<typename T>
    struct DescriptorInfo {};

#define REG_DESCRIPTOR(type_, version_) \
template<> \
struct DescriptorInfo<type_> { \
    static inline constexpr Version version{version_}; \
    static inline const char* type = #type_; \
}; \
template<> \
struct DescriptorInfo<const type_> { \
    static inline constexpr Version version{version_}; \
    static inline const char* type = #type_; \
};


}

#endif //PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_BASE_H_
