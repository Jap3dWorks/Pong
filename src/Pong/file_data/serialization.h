//
// Created by Jordi on 8/13/2022.
//

#ifndef GL_TEST_SERIALIZATION_H
#define GL_TEST_SERIALIZATION_H

#include <iostream>
#include <ostream>
#include "Utils/type_conditions.h"
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
        inline void write(std::ostream &ostream, char *ptr, std::streamsize size) {
            ostream.write(ptr, size);
        }
    }

    template<typename T>
    static inline void write(std::ostream &ostream, const T &value) {
        write(ostream, (char *) &value, sizeof(value));
    }

    template<typename T>
    static inline void write(std::ostream& ostream, const std::vector<T>& value) {
        for(auto& it: value) {
            write(ostream, *it);
        }
    }

    static inline void write(std::ostream& ostream, const std::string& value) {
        write(ostream, (char*)value.data(), value.size());
    }

    class OSSerializer {
    public:
        using ostream = std::ostream;

    private:
        ostream& os_;

    public:
        explicit OSSerializer(std::ostream& os): os_(os) {}

        template<typename T>
        OSSerializer& operator&(const T& other) {
            write(os_, other);
            return *this;
        }

        template<typename T>
        OSSerializer& operator<<(const T& other) {
            serialize(*this, other, serialized_version<T>::version);
            return *this;
        }

        ostream& get() {
            return os_;
        }
    };



    class ISSerializer {

    };

}
#endif //GL_TEST_SERIALIZATION_H
