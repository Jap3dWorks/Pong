//
// Created by Jordi on 8/13/2022.
//

#ifndef GL_TEST_SERIALIZATION_H
#define GL_TEST_SERIALIZATION_H

#include "Pong/file_data/reflectable.h"
#include <iostream>
#include <ostream>
#include "Utils/type_conditions.h"
#include <functional>
#include <vector>
#include <concepts>
#include <string>
#include <optional>


#define SERIALIZABLE REFLECTABLE

#define SERIAL_CLASS_VERSION(type_, version_) \
template<> \
struct serialized_version<type_> { \
    static inline constexpr Version version{version_}; \
    static inline const char* type = #type_; \
}; \
template<> \
struct serialized_version<const type_> { \
    static inline constexpr Version version{version_}; \
    static inline const char* type = #type_; \
};

#define IMPL_SERIALIZE(type_) \
    template<typename Archive> \
    inline void serialize(Archive &ar,\
        type_ &value, \
        const Version& version) { \
        serialize_fields(ar, value); \
    }


namespace Pong::serializer {
    using Version=std::optional<uint32_t>;

    template<typename Archive>
    struct serialize_visitor {
        Archive &ar;

        explicit serialize_visitor(Archive& archive):
                ar(archive) {}

        template<typename ReflectedField>
        inline void operator()(ReflectedField f) {
            serialize(ar, f.get(), {});
        }
    };

    template<typename Archive, typename Reflected>
    void serialize_fields(Archive& ar, Reflected& x) {
        visit_each(x, serialize_visitor(ar));
    }

    template<typename Archive, typename Value>
    inline void serialize(Archive &ar, Value &value, const Version& version) {
        ar & value;
    }

    template<typename Archive>
    inline void serialize(Archive &ar, RegId &uid, const Version& version) {
        ar & to_integer(uid);
    }

    template<typename T>
    struct serialized_version {};

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

        for(auto& it: value) {
            serialize(ar, *it, {});
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
        auto hasval = value.has_value();
        load(ar, (char*)&hasval, sizeof(hasval));

        if (hasval) {
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
    explicit Serial(stream_reference& os_): stream_(os_) {} \
    template<typename T> \
    auto& operator<<(T& other) { \
        auto version = serialized_version<T>::version; \
        auto type_name = std::string(serialized_version<T>::type); \
        auto header = Header{*version, type_name, 0};       \
        serialize(*this, header, version); \
        serialize(*this, other, version); \
        return *this; \
    } \
    auto& get() { \
        return stream_; \
    }

    class OSSerializer {
    SERIALIZER_COMMON(OSSerializer, std::ostream);

    public:
        template<typename T>
        auto &operator&(const T &other) {
            save(*this, other);
            return *this;
        }
    };


    class ISSerializer{
    SERIALIZER_COMMON(ISSerializer, std::istream);

    public:
        template<typename T>
        auto &operator&(T &other) {
            load(stream_, other);
            return *this;
        }
    };

}

#endif //GL_TEST_SERIALIZATION_H
