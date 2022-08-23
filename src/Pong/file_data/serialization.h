//
// Created by Jordi on 8/13/2022.
//

#ifndef GL_TEST_SERIALIZATION_H
#define GL_TEST_SERIALIZATION_H

#include "Pong/file_data/serialize_types.h"
#include "Pong/file_data/reflectable.h"
#include "Pong/file_data/serialize_functions.h"
#include "Pong/file_data/data_header.h"
#include <iostream>
#include <ostream>
#include "Utils/type_conditions.h"
#include <functional>
#include <vector>
#include <concepts>
#include <string>
#include <optional>

namespace Pong::serializer {


#define SERIALIZER_COMMON(Serial, Stream) \
protected: \
    public: \
    using stream_type = Stream; \
    using stream_reference = Stream&; \
    protected: \
    stream_reference stream_; \
    public: \
    explicit Serial(stream_reference& fsparam_): stream_(fsparam_) {} \
    auto& get() { \
        return stream_; \
    }


    class OSSerializer {
    SERIALIZER_COMMON(OSSerializer, std::ofstream);

    public:

        Version version{};

        template<typename T>
        auto &operator<<(T &descriptor) {
            version = descriptor_data<T>::version;
            descriptor.data.header.version = version;
            serialize(*this, descriptor, version);
            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::save(*this, other, version);
            return *this;
        }
    };

    class ISSerializer{
    SERIALIZER_COMMON(ISSerializer, std::ifstream);

    public:
        Version version{};

        template<typename T>
        auto &operator>>(T &other) {
            serialize(*this, other.data.header, {});
            version = other.data.header.version;
//            stream_.seekg(0, stream_type::beg);
            serialize(*this, other.data.data, version);

            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::load(*this, other, version);
            return *this;
        }

    };



}

#endif //GL_TEST_SERIALIZATION_H
