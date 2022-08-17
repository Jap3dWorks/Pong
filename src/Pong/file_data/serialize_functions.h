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


}


#endif //GL_TEST_SERIALIZE_FUNCTIONS_H
