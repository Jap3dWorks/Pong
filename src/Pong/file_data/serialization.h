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
        template<typename T>
        auto &operator<<(T &other) {
            assert(strlen(descriptor_data<T>::type) <= P_MAX_SERIALIZER_NAME_LENGTH);

            auto version = descriptor_data<T>::version;
            auto header = FileHeader{0, version};

            auto size_serializer = SizeSerializer();

            serialize(size_serializer, other, version);
            header.data_size = size_serializer.get();
            size_serializer.clear();

            serialize(*this, header, version);

            // serialize each struct with its size.
            serialize(*this, other, version);
            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::save(*this, other, {});
            return *this;
        }
    };

    class ISSerializer{
    SERIALIZER_COMMON(ISSerializer, std::ifstream);

    public:
        template<typename T>
        auto &operator>>(T &other) {
            auto header = FileHeader{};
            serialize(*this, header, {});

            auto version = header.version;
            serialize(*this, other, version);

            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::load(*this, other, {});
            return *this;
        }

    };



}

#endif //GL_TEST_SERIALIZATION_H
