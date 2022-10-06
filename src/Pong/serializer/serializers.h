//
// Created by Jordi on 8/13/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_SERIALIZERS_H_
#define PONG_SRC_PONG_SERIALIZER_SERIALIZERS_H_

#include <iostream>
#include <ostream>
#include <functional>
#include <vector>
#include <concepts>
#include <string>
#include <optional>

#include "Utils/type_conditions.h"
#include "Pong/serializer/serial_types.h"
#include "Pong/serializer/reflectable.h"
#include "Pong/serializer/serial_functions.h"
#include "Pong/serializer/header_data.h"


namespace pong::serializer {


#define SERIALIZER_COMMON(type_name_) \
    private: \
    using self_type=type_name_;  \
    self_type& self{*this};


    template<typename stream_t>
    class BaseSerializer {
    public:
        using StreamType = stream_t;
        using StreamReference = StreamType &;
    protected:
        StreamReference stream;

    public:
        explicit BaseSerializer(StreamReference &stream_p):
            stream(stream_p) {}

        auto &get() {
            return stream;
        }

    public:
        Version version{};

        template<typename Descriptor>
        inline void update_descriptor_name(Descriptor& descriptor) {
            strncpy((char*) descriptor.data.header.type_name,
                    descriptor_info<Descriptor>::type,
                    strlen(descriptor_info<Descriptor>::type));
        }

        template<typename Descriptor>
        inline void collect_version(std::optional<Descriptor *> descriptor) {
            version = descriptor_info<Descriptor>::version;
            if (descriptor) {
                descriptor.value()->data.header.version = version;
            }
        }

        template<typename Archive>
        void collect_version(Archive &ar, std::ifstream &stream_p) {
            auto header = FileHeader();
            serialize(ar, header, {});
            version = header.version;
            stream_p.seekg(0, StreamType::beg);
        }

    };

    class OBaseSerializer : public BaseSerializer<std::ofstream> {
        using BaseSerializer::BaseSerializer;
    };

    class OSerializer: public OBaseSerializer {
    SERIALIZER_COMMON(OSerializer);
    using OBaseSerializer::OBaseSerializer;

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

    class IBaseSerializer : public BaseSerializer<std::ifstream> {
    public:
        using BaseSerializer<std::ifstream>::BaseSerializer;

    };

#define IBASE_SERIALIZER_CONSTRUCTORS() \
    using IBaseSerializer::IBaseSerializer; \
    public: \
    template<typename Descriptor> \
    auto &operator>>(Descriptor &descriptor) { \
        collect_version(self, stream); \
        serialize(*this, descriptor, version); \
        return *this;\
    }


    class ISerializer: public IBaseSerializer {
    SERIALIZER_COMMON(ISerializer);
    IBASE_SERIALIZER_CONSTRUCTORS();

    public:
        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::load(*this, other, version);
            return *this;
        }
    };


    class IHeaderSerializer: public IBaseSerializer {
        SERIALIZER_COMMON(IHeaderSerializer);
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
    class IRegIdSerializer: public IBaseSerializer {
    public:
        using HeadedDt = HeadedData<Header<data_type>, data_type>;
    private:
        RegId reg_id_;
        bool found_{false};

    SERIALIZER_COMMON(IRegIdSerializer);
    IBASE_SERIALIZER_CONSTRUCTORS();

    public:
        IRegIdSerializer(StreamReference stream,
                         RegId reg_id) :
            IBaseSerializer(stream),
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
            auto pos = stream.tellg();

            auto input_serializer = ISerializer(stream);
            serialize(input_serializer, header, version);
            stream.seekg(pos, StreamType::beg);

            return header.reg_id;
        }

        auto &operator&(HeadedDt& other) {
            // get header data
            auto reg_id = get_reg_id(self, other.header, version);

            if (reg_id == reg_id_) {
                found_ = true;
                SaveLoadSize<HeadedDt>::load(*this, other, version);
            }
            else {
                found_ = false;
                SaveLoadSize<HeadedDt>::jump(*this, other, version);
            }
            return *this;
        }

    };


//    class IComponentSerializer: public IBaseSerializer {
//    SERIALIZER_COMMON(IComponentSerializer);
//    IBASE_SERIALIZER_CONSTRUCTORS();
//
//
//    };


}

#endif //PONG_SRC_PONG_SERIALIZER_SERIALIZERS_H_
