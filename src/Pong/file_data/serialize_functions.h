//
// Created by Jordi on 8/16/2022.
//

#ifndef GL_TEST_SERIALIZE_FUNCTIONS_H
#define GL_TEST_SERIALIZE_FUNCTIONS_H
#include "Pong/core/geometry_data.h"
#include <iostream>

#define SERIALIZABLE REFLECTABLE


#define IMPL_SERIALIZE(type_) \
    template<typename Archive> \
    void serialize(Archive &ar,\
        type_ &value, \
        const Version& version) { \
        serialize_fields(ar, value); \
    }


#define REG_DESCRIPTOR(type_, version_) \
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


    template<typename T>
    struct serialized_version {};


    template<typename Archive, typename Value>
    void serialize(Archive &ar, Value &value, const Version& version) {
        ar & value;
    }

    template<typename Archive>
    void serialize(Archive &ar, Mesh &value, const Version& version) {
        ar & value.vertices;
        ar & value.indices;
    }

    template<typename Archive>
    void serialize(Archive &ar, Curve &value, const Version& version) {
        ar & value.points;
    }


    // save load and size functions
    // save types
    template<typename Archive>
    inline void save(Archive &ar, char *ptr, std::streamsize size) {
        ar.get().write(ptr, size);
    }

    template<typename Archive, typename U>
    static inline void save(Archive &ar, const U &value) {
        save(ar, (char *) &value, sizeof(value));
    }

    template<typename Archive, typename U>
    static inline void save(Archive& ar, const std::vector<U>& value) {
        size_t size = value.size();
        save(ar, (char *) &size, sizeof(size));

        for (uint32_t i=0; i<size; ++i) {
            serialize(ar, value[i], {});
        }
    }

    template<typename Archive>
    static inline void save(Archive& ar, const std::string& value) {
        size_t size = value.size();
        save(ar, (char *) &size, sizeof(size));
        save(ar, (char *) value.data(), size);
    }

    template<typename Archive, typename U>
    static inline void save(Archive& ar, const std::reference_wrapper<U>& value) {
        serialize(ar, value.get(), {});
    }

    template<typename Archive, typename U>
    static inline void save(Archive& ar, const std::optional<U>& value) {
        auto hasval = value.has_value();
        save(ar, (char*)&hasval, sizeof(hasval));

        if (hasval) {
            serialize(ar, *value, {});
        }
    }

    // load types
    template<typename Archive>
    inline void load(Archive &ar, char *ptr, std::streamsize size) {
        ar.get().read(ptr, size);
    }

    template<typename Archive, typename T>
    static inline void load(Archive &ar, T &value) {
        load(ar, (char *) &value, sizeof(value));
    }

    template<typename Archive, typename T>
    static inline void load(Archive &ar, std::vector<T> &value) {
        size_t vector_size;
        load(ar, (char *) &vector_size, sizeof(size_t));
        value.resize(vector_size);

        for(uint32_t i=0; i<vector_size; ++i) {
            serialize(ar, value[i], {});
        }
    }

    template<typename Archive>
    static inline void load(Archive& ar, std::string& value) {
        size_t string_size;
        load(ar, (char *) &string_size, sizeof(size_t));
        value.resize(string_size);

        load(ar, (char *) value.data(), string_size);
    }

    template<typename Archive, typename T>
    static inline void load(Archive& ar, std::reference_wrapper<T>& value) {
        serialize(ar, value.get(), {});
    }

    template<typename Archive, typename T>
    static inline void load(Archive& ar, std::optional<T>& value) {
        bool hasval;
        load(ar, (char*)&hasval, sizeof(hasval));
        if (hasval) {
            value.emplace();
            serialize(ar, *value, {});
        }
    }


    // size types
    template<typename Archive, typename T>
    inline void size(Archive& ar, const T& value) {
        ar += sizeof(value);
    }

    template<typename Archive, typename T>
    inline void size(Archive& ar, std::vector<T> &value) {
        ar += sizeof(size_t);

        auto v_size = value.size();
        if (v_size) {
            auto temp_ar = Archive();
            serialize(temp_ar, value[0], {});

            ar += (v_size * temp_ar);
        }
    }

    template<typename Archive>
    inline void size(Archive& ar, const std::string& value) {
        size_t size = value.size();
        ar += sizeof(size);
        ar += size;
    }

    template<typename Archive, typename T>
    inline void size(Archive & ar, std::reference_wrapper<T>& value) {
        serialize(ar, value.get(), {});
    }

    template<typename Archive, typename T>
    inline void size(Archive& ar, std::optional<T>& value) {
        auto hasval = value.has_value();
        ar += sizeof(hasval);

        if (hasval) {
            serialize(ar, *value, {});
        }
    }

}


#endif //GL_TEST_SERIALIZE_FUNCTIONS_H
