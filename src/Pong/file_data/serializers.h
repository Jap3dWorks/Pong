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

        template<typename Descriptor>
        inline void collect_version(std::optional<Descriptor*> descriptor) {
            version = descriptor_data<Descriptor>::version;
            if(descriptor) {
                descriptor.value()->data.header.version = version;
            }
        }

        template<typename Descriptor>
        inline void update_descriptor_name(Descriptor& descriptor) {
            strncpy((char*) descriptor.data.header.type_name,
                    descriptor_data<Descriptor>::type,
                    strlen(descriptor_data<Descriptor>::type));
        }

        template<typename Descriptor>
        auto &operator<<(Descriptor &descriptor) {
            collect_version<Descriptor>(&descriptor);
            update_descriptor_name(descriptor);

            serialize(*this, descriptor, version);
            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::save(*this, other, version);
            return *this;
        }
    };


    template<typename T>
    struct DataLocation {
        RegId reg_id{0};
        std::string file_path{};
        size_t offset{0};
    };


    class ISSerializer{
    SERIALIZER_COMMON(ISSerializer, std::ifstream);

    public:
        Version version{};

        void collect_version() {
            auto header = FileHeader();
            serialize(*this, header, {});
            version = header.version;
            stream_.seekg(0, stream_type::beg);
        }

        template<typename Descriptor>
        auto &operator>>(Descriptor &descriptor) {
            collect_version();
            serialize(*this, descriptor, version);

            return *this;
        }

        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::load(*this, other, version);
            return *this;
        }

        template<typename Descriptor, typename T>
        void load_selection(Descriptor &descriptor, DataLocation<T> selection) {
            collect_version();
            stream_.seekg(selection.offset, stream_type::beg);
            SaveLoadSize<T>::load(descriptor, selection, version);
        }

    };

}

#endif //GL_TEST_SERIALIZATION_H
