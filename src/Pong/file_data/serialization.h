//
// Created by Jordi on 8/13/2022.
//

#ifndef GL_TEST_SERIALIZATION_H
#define GL_TEST_SERIALIZATION_H

#include <iostream>
#include <ostream>
#include "Utils/type_conditions.h"
#include <functional>
#include <vector>
#include <concepts>
#include <string>
#include <optional>


#define SERIAL_CLASS_VERSION(class_, version_)                                \
template<>                                                                    \
struct Pong::serializer::serialized_version<class_> {                         \
    static inline constexpr Version version{version_};                        \
};

namespace Pong::serializer {
    using Version=std::optional<uint32_t>;

    template<typename T>
    struct serialized_version {
        static inline constexpr Version version{};
    };


    template<typename Archive, typename Value>
    void serialize(Archive &ar, Value &value, const Version& file_version) {
        ar & value;
    }

    namespace {
        inline void save(std::ostream &ostream, char *ptr, std::streamsize size) {
            ostream.write(ptr, size);
        }

        inline void load(std::istream &istream, char *ptr, std::streamsize size) {
            istream.read(ptr, size);
        }
    }

    // save types
    template<typename T>
    static inline void save(std::ostream &ostream, const T &value) {
        save(ostream, (char *) &value, sizeof(value));
    }

    template<typename T>
    static inline void save(std::ostream& ostream, const std::vector<T>& value) {
        size_t size = value.size();
        save(ostream, (char *) &size, sizeof(size));

        for(auto& it: value) {
            save(ostream, *it);
        }
    }

    static inline void save(std::ostream& ostream, const std::string& value) {
        size_t size = value.size();
        save(ostream, (char *) &size, sizeof(size));
        save(ostream, (char *) value.data(), size);
    }

    template<typename T>
    static inline void save(std::ostream& ostream, const std::reference_wrapper<T>& value) {
        save(ostream, (char*)&value.get(), sizeof(T));
    }

    template<typename T>
    static inline void save(std::ostream& ostream, const std::optional<T>& value) {
        auto hasval = value.has_value();
        save(ostream, (char*)&hasval, sizeof(hasval));

        if (hasval) {
            save(ostream, *value);
        }
    }


    // load types
    template<typename T>
    static inline void load(std::istream &istream, T &value) {
        load(istream, (char *) &value, sizeof(value));
    }

    template<typename T>
    static inline void load(std::istream &istream, std::vector<T> &value) {
        size_t vector_size;
        load(istream, (char *) &vector_size, sizeof(size_t));
        value.resize(vector_size);

        for(auto& it: value) {
            load(istream, *it);
        }
    }

    static inline void load(std::istream& istream, std::string& value) {
        size_t string_size;
        load(istream, (char *) &string_size, sizeof(size_t));
        value.resize(string_size);

        load(istream, (char *) value.data(), string_size);
    }

    template<typename T>
    static inline void load(std::istream& istream, std::reference_wrapper<T>& value) {
        load(istream, (char*)&value.get(), sizeof(T));
    }

    template<typename T>
    static inline void load(std::istream& istream, std::optional<T>& value) {
        auto hasval = value.has_value();
        load(istream, (char*)&hasval, sizeof(hasval));

        if (hasval) {
            load(istream, *value);
        }
    }

    struct Header {
        uint32_t version;
        std::string type;
        uint32_t size;
    };


    template<typename Stream>
    class BaseSerializer {
    protected:
        using base_serializer = BaseSerializer<Stream>;

    public:
        using stream_type = Stream;
        using stream_reference = Stream&;

    protected:
        stream_reference stream_;

    public:
        explicit BaseSerializer(stream_reference& os_): stream_(os_) {}


        template<typename T>
        auto& operator<<(const T& other) {
            serialize(*this, other, serialized_version<T>::version);
            return *this;
        }

        auto& get() {
            return stream_;
        }
    };


    class OSSerializer : public BaseSerializer<std::ostream> {
    public:
        using base_serializer::base_serializer;

    public:
        template<typename T>
        auto &operator&(const T &other) {
            save(stream_, other);
            return *this;
        }
    };


    class ISSerializer : public BaseSerializer<std::istream> {
    public:
        using base_serializer::base_serializer;

    public:
        template<typename T>
        auto &operator&(const T &other) {
            load(stream_, other);
            return *this;
        }
    };


    template<typename Archive>
    struct serialize_visitor {
        Archive &ar;

        explicit serialize_visitor(Archive& asset_serializer_):
                ar(asset_serializer_) {}

        template<typename ReflectedField>
        void operator()(ReflectedField f) {
            ar & f.get();
        }
    };

    template<typename Archive, typename Reflected>
    void serialize_fields(Archive& ar, Reflected& x) {
        visit_each(x, serialize_visitor(ar));
    }

#define SERIALIZABLE REFLECTABLE

#define SERIALIZABLE_IMPL(class_) \
    template<typename Archive> \
    void serialize(Archive &ar, class_ &value, const Version& file_version) { \
        serialize_fields(value, ar);                              \
    }
}

#endif //GL_TEST_SERIALIZATION_H
