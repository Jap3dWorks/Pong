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


#define SERIALIZER_COMMON(type_name_) \
    private: \
    using self_type=type_name_;  \
    self_type& self{*this};


    template<typename stream_t>
    class base_serializer_ {
    public:
        using stream_type = stream_t;
        using stream_reference = stream_type &;
    protected:
        stream_reference stream_;

    public:
        explicit base_serializer_(stream_reference &stream):
        stream_(stream) {}

        auto &get() {
            return stream_;
        }

    public:
        Version version{};

        template<typename Descriptor>
        inline void update_descriptor_name(Descriptor& descriptor) {
            strncpy((char*) descriptor.data.header.type_name,
                    descriptor_data<Descriptor>::type,
                    strlen(descriptor_data<Descriptor>::type));
        }

        template<typename Descriptor>
        inline void collect_version(std::optional<Descriptor *> descriptor) {
            version = descriptor_data<Descriptor>::version;
            if (descriptor) {
                descriptor.value()->data.header.version = version;
            }
        }

        template<typename Archive>
        void collect_version(Archive &ar, std::ifstream &stream) {
            auto header = FileHeader();
            serialize(ar, header, {});
            version = header.version;
            stream.seekg(0, stream_type::beg);
        }

    };

    class obase_serializer : public base_serializer_<std::ofstream> {
        using base_serializer_::base_serializer_;
    };

    class OSSerializer: public obase_serializer {
    SERIALIZER_COMMON(OSSerializer);
    using obase_serializer::obase_serializer;

    public:
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

    class ibase_serializer_ : public base_serializer_<std::ifstream> {
    public:
        using base_serializer_<std::ifstream>::base_serializer_;
    };

#define IBASE_SERIALIZER_CONSTRUCTORS() \
    using ibase_serializer_::ibase_serializer_; \
    public: \
    template<typename Descriptor> \
    auto &operator>>(Descriptor &descriptor) { \
        collect_version(self, stream_); \
        serialize(*this, descriptor, version); \
        return *this;\
    }

    class ISSerializer: public ibase_serializer_ {
    SERIALIZER_COMMON(ISSerializer);
    IBASE_SERIALIZER_CONSTRUCTORS();

    public:
        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::load(*this, other, version);
            return *this;
        }
    };

    class ISHeaderSerializer: public ibase_serializer_ {
        SERIALIZER_COMMON(ISHeaderSerializer);
        IBASE_SERIALIZER_CONSTRUCTORS();

        template<typename T>
        auto &operator&(T &other)  {
            SaveLoadSize<T>::jump(*this, other, version);
            return *this;
        }

        template<typename T>
        auto &operator&(Header<T>& other) {
            SaveLoadSize<T>::load(*this, other, version);
            return *this;
        }
    };

    template<typename data_type>
    class IRegIdSerializer: public ibase_serializer_ {
    public:
        using headed_dt = HeadedData<Header<data_type>, data_type>;
    private:
        RegId reg_id_;
        bool found_{false};

    SERIALIZER_COMMON(IRegIdSerializer);
    IBASE_SERIALIZER_CONSTRUCTORS();

    public:
        IRegIdSerializer(stream_reference stream,
                         RegId reg_id) :
                ibase_serializer_(stream),
                reg_id_(reg_id) {}

        [[nodiscard]] RegId get_reg_id() const {
            return reg_id_;
        }

        void set_reg_id(const RegId &reg_id) {
            reg_id_ = reg_id;
        }

        template<typename T>
        auto &operator&(T &other)  {
            if (found_) {
                SaveLoadSize<T>::load(*this, other, version);
            }
            else {
                SaveLoadSize<T>::jump(*this, other, version);
            }
            return *this;
        }

        template<typename Archive, typename T>
        RegId get_reg_id(Archive &ar, Header<T> header, Version& version) {

            auto pos = stream_.tellg();

            auto input_serializer = ISSerializer(stream_);
            serialize(input_serializer, header, version);
            stream_.seekg(pos, stream_type::beg);

            return header.reg_id;
        }

        auto &operator&(headed_dt& other) {
            // get header data
            auto reg_id = get_reg_id(self, other.header, version);

            if (reg_id == reg_id_) {
                found_ = true;
                SaveLoadSize<headed_dt>::load(*this, other, version);
            }
            else {
                found_ = false;
                SaveLoadSize<headed_dt>::jump(*this, other, version);
            }
            return *this;
        }
    };
}

#endif //GL_TEST_SERIALIZATION_H
