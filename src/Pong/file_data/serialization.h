//
// Created by Jordi on 8/13/2022.
//

#ifndef GL_TEST_SERIALIZATION_H
#define GL_TEST_SERIALIZATION_H

#include "Pong/file_data/reflectable.h"
#include "Pong/file_data/serialize_functions.h"
#include <iostream>
#include <ostream>
#include "Utils/type_conditions.h"
#include <functional>
#include <vector>
#include <concepts>
#include <string>
#include <optional>


namespace Pong::serializer {

    struct Header {
        SERIALIZABLE (
                FIELD(Version, version),
                FIELD(std::string, type),
                FIELD(uint32_t, size)
        )
    };
    IMPL_SERIALIZE(Header);

    template<typename T>
    inline void save(T &ar, char *ptr, std::streamsize size) {
        ar.get().write(ptr, size);
    }

    template<typename T>
    inline void load(T &ar, char *ptr, std::streamsize size) {
        ar.get().read(ptr, size);
    }

    // save types
    template<typename T, typename U>
    static inline void save(T &ar, const U &value) {
        save(ar, (char *) &value, sizeof(value));
    }

    template<typename T, typename U>
    static inline void save(T& ar, const std::vector<U>& value) {
        size_t size = value.size();
        save(ar, (char *) &size, sizeof(size));

        for (uint32_t i=0; i<size; ++i) {
            serialize(ar, value[i], {});
        }
    }

    template<typename T>
    static inline void save(T& ar, const std::string& value) {
        size_t size = value.size();
        save(ar, (char *) &size, sizeof(size));
        save(ar, (char *) value.data(), size);
    }

    template<typename T, typename U>
    static inline void save(T& ar, const std::reference_wrapper<U>& value) {
        serialize(ar, value.get(), {});
    }

    template<typename T, typename U>
    static inline void save(T& ar, const std::optional<U>& value) {
        auto hasval = value.has_value();
        save(ar, (char*)&hasval, sizeof(hasval));

        if (hasval) {
            serialize(ar, *value, {});
        }
    }

    // load types
    template<typename T, typename U>
    static inline void load(T &ar, U &value) {
        load(ar, (char *) &value, sizeof(value));
    }

    template<typename T, typename U>
    static inline void load(T &ar, std::vector<U> &value) {
        size_t vector_size;
        load(ar, (char *) &vector_size, sizeof(size_t));
        value.resize(vector_size);

        for(uint32_t i=0; i<vector_size; ++i) {
            serialize(ar, value[i], {});
        }
    }

    template<typename T>
    static inline void load(T& ar, std::string& value) {
        size_t string_size;
        load(ar, (char *) &string_size, sizeof(size_t));
        value.resize(string_size);

        load(ar, (char *) value.data(), string_size);
    }

    template<typename T, typename U>
    static inline void load(T& ar, std::reference_wrapper<U>& value) {
        serialize(ar, value.get(), {});
    }

    template<typename T, typename U>
    static inline void load(T& ar, std::optional<U>& value) {
        bool hasval;
        load(ar, (char*)&hasval, sizeof(hasval));
        if (hasval) {
            value.emplace();
            serialize(ar, *value, {});
        }
    }

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
            auto version = serialized_version<T>::version;
            auto type_name = std::string(serialized_version<T>::type);
            auto header = Header{version, type_name, 0};
            serialize(*this, header, version);
            serialize(*this, other, version);
            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            save(*this, other);
            return *this;
        }
    };


    class ISSerializer{
    SERIALIZER_COMMON(ISSerializer, std::ifstream);

    public:
        template<typename T>
        auto &operator>>(T &other) {
            auto header = Header{};

            serialize(*this, header, {});
            auto version = header.version;
            serialize(*this, other, version);

            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            load(*this, other);
            return *this;
        }

    };

}

#endif //GL_TEST_SERIALIZATION_H
