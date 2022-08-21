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
                FIELD(size_t, size),
                FIELD(Version, version),
                FIELD(char, type[16])
        )
    };
    IMPL_SERIALIZE(Header);


    class SizeSerializer {
    private:
        size_t size_{0};

    public:
        template<typename T>
        auto &operator&(T &other) {
            size(*this, other);
            return *this;
        }

        [[nodiscard]] auto get() const noexcept {
            return size_;
        }

        auto clear() noexcept {
            size_ = 0;
        }

        template<typename T>
        SizeSerializer& operator+=(const T& other) {
            size_ += other;
            return *this;
        }

        template<typename T>
        SizeSerializer operator+(const T& other) {
            auto temp = size_ + other;
            return temp;
        }

        template<typename T>
        SizeSerializer &operator*=(const T &other) {
            size_ *= other;
            return *this;
        }

        template<typename T>
        SizeSerializer operator*(const T& other) {
            auto temp = *this;
            temp += other;
            return temp;
        }

        template<typename T>
        friend T& operator+=(T& left_, const SizeSerializer& right_);

        template<typename T>
        friend T operator+(T& left_, const SizeSerializer& right_);

        template<typename T>
        friend T& operator*=(T& left_, const SizeSerializer& right_);

        template<typename T>
        friend T operator*(T& left_, const SizeSerializer& right_);

    };

    template<typename T>
    T &operator+=(T& left_, const SizeSerializer& right_) {
        left_ += right_.size_;
        return left_;
    }

    template<typename T>
    T operator+(T& left_, const SizeSerializer& right_) {
        auto temp = left_ + right_.size_;
        return temp;
    }

    template<typename T>
    T& operator*=(T& left_, const SizeSerializer& right_) {
        left_ *= right_.size_;
        return left_;
    }

    template<typename T>
    T operator*(T& left_, const SizeSerializer& right_) {
        auto temp = left_ * right_.size_;
        return temp;
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
            auto size_serializer = SizeSerializer();

            serialize(size_serializer, header, version);
            header.size = size_serializer.get();
            size_serializer.clear();
            serialize(*this, header, version);

            // serialize each struct with its size.
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
