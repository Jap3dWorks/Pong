//
// Created by Jordi on 8/16/2022.
//

#ifndef GL_TEST_SERIALIZE_FUNCTIONS_H
#define GL_TEST_SERIALIZE_FUNCTIONS_H
#include "Pong/core/geometry_data.h"
#include "Pong/file_data/serialize_types.h"
#include "Pong/config/config.h"
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
struct descriptor_data<type_> { \
    static inline constexpr Version version{version_}; \
    static inline const char* type = #type_; \
}; \
template<> \
struct descriptor_data<const type_> { \
    static inline constexpr Version version{version_}; \
    static inline const char* type = #type_; \
};


namespace Pong::serializer {
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
    struct descriptor_data {};


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


    template<typename T>
    struct SaveLoadSize {
        template<typename Archive>
        static inline void save(Archive &ar, const T &value, const Version& version) {
            ar.get().write((char *) &value, sizeof(value));
        }

        template<typename Archive>
        static inline void load(Archive &ar, T &value, const Version& version) {
            ar.get().read((char *) &value, sizeof(value));
        }

        template<typename Archive>
        static inline void size(Archive& ar, const T& value, const Version& version) {
            ar += sizeof(value);
        }
    };

    template<typename T>
    struct SaveLoadSize<std::vector<T>> {
        template<typename Archive>
        static inline void save(Archive &ar, const T &value, const Version& version) {
            data_size_t size = value.size();
            SaveLoadSize<data_size_t>::save(ar, size, version);

            for (uint32_t i=0; i<size; ++i) {
                serialize(ar, value[i], version);
            }
        }

        template<typename Archive>
        static inline void load(Archive &ar, T &value, const Version& version) {
            data_size_t vector_size;
            SaveLoadSize<data_size_t>::load(ar, vector_size, version);
            value.resize(vector_size);
            for(uint32_t i=0; i<vector_size; ++i) {
                serialize(ar, value[i], version);
            }
        }

        template<typename Archive>
        static inline void size(Archive& ar, const T& value, const Version& version) {
            ar += sizeof(data_size_t);

            auto v_size = value.size();
            auto temp_ar = Archive();

            for (data_size_t i=0; i<v_size; ++i) {
                serialize(temp_ar, value[i], version);
                ar += temp_ar;
                temp_ar.clear();
            }
        }
    };

    template<>
    struct SaveLoadSize<std::string> {
        template<typename Archive>
        static inline void save(Archive& ar, const std::string& value, const Version& version) {
            data_size_t size = value.size();
            SaveLoadSize<data_size_t>::save(ar, size, version);

            ar.get().write((char *) value.data(), size);
        }

        template<typename Archive>
        static inline void load(Archive& ar, std::string& value, const Version& version) {
            data_size_t string_size;
            SaveLoadSize<data_size_t>::load(ar, string_size, version);
            value.resize(string_size);

            ar.get().read((char *) value.data(), string_size);
        }

        template<typename Archive>
        inline void size(Archive& ar, const std::string& value, const Version& version) {
            data_size_t size = value.size();
            ar += sizeof(data_size_t);
            ar += size;
        }
    };

    template<typename T>
    struct SaveLoadSize<std::reference_wrapper<T>> {

        using type = std::reference_wrapper<T>;

        template<typename Archive>
        static inline void save(Archive& ar, const type& value, const Version& version) {
            serialize(ar, value.get(), version);
        }

        template<typename Archive>
        static inline void load(Archive& ar, type& value, const Version& version) {
            serialize(ar, value.get(), version);
        }

        template<typename Archive>
        inline void size(Archive & ar, type& value, const Version& version) {
            serialize(ar, value.get(), version);
        }
    };

    template<typename T>
    struct SaveLoadSize<std::optional<T>> {
        using type = std::optional<T>;

        template<typename Archive>
        static inline void save(Archive& ar, const type& value, const Version& version) {
            auto hasval = value.has_value();
            SaveLoadSize<decltype(hasval)>::save(ar, hasval, version);

            if (hasval) {
                serialize(ar, *value, version);
            }
        }

        template<typename Archive>
        static inline void load(Archive& ar, type& value, const Version& version) {
            bool hasval;
            SaveLoadSize<bool>::load(ar, hasval, version);
            if (hasval) {
                value.emplace();
                serialize(ar, *value, version);
            }
        }

        template<typename Archive>
        inline void size(Archive& ar, type& value, const Version& version) {
            auto hasval = value.has_value();
            ar += sizeof(hasval);

            if (hasval) {
                serialize(ar, *value, version);
            }
        }
    };

}


#endif //GL_TEST_SERIALIZE_FUNCTIONS_H
