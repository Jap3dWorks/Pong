//
// Created by Jordi on 8/21/2022.
//

#ifndef PONG_SRC_PONG_SERIAL_DATA_HEADER_DATA_H_
#define PONG_SRC_PONG_SERIAL_DATA_HEADER_DATA_H_

#include "Pong/serial_data/reflectable.h"
#include "Pong/serial_data/serial_functions.h"
#include "Pong/serial_data/serial_types.h"
#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>

namespace pong::serializer {
    struct FileHeader_t {};

    template<typename T>
    struct data_header_ {
        static inline constexpr DataSize header_size{
            sizeof(RegId) +
                sizeof(DataSize)
        };

        SERIALIZABLE (
            FIELD(RegId, reg_id, 0),
            FIELD(DataSize, data_size, 0)
        )

        using Type = T;
    };

    template<>
    struct data_header_<FileHeader_t> {
        static inline constexpr DataSize header_size{
            (sizeof(DataSize)*2) + P_MAX_SERIALIZER_NAME_LENGTH};

        DataSize data_size{0};
        Version version{};
        const char type_name[P_MAX_SERIALIZER_NAME_LENGTH]{};

        using Type = FileHeader_t;
    };

    template<typename T>
    using Header = data_header_<T>;
    using FileHeader = data_header_<FileHeader_t>;

    template<typename Archive, typename T>
    void serialize(Archive &ar, Header<T> &value, const Version &version) {
        serialize_fields(ar, value);
    }

    template<typename Archive>
    void serialize(Archive &ar, FileHeader &value, const Version &version) {
        ar & value.data_size;
        ar & value.version;
        ar & value.type_name;
    }

    template<typename Header_, typename Data_, typename DataType_=Any_t>
    struct HeadedData {
        Header_ header{};
        Data_ data{};
    };

    template<typename Header_, typename Data_>
    struct HeadedData<Header_, Data_, reference_data_t> {
        Header_ header{};
        Data_ &data{};
    };

    template<typename T>
    struct SaveLoadSize<Header<T>> {
        using Type = Header<T>;

        template<typename Archive>
        static inline void save(Archive &ar, Type &value, const Version &version) {
            serialize(ar, value, version);
        }

        template<typename Archive>
        static inline void load(Archive &ar, Type &value, const Version &version) {
            serialize(ar, value, version);
        }

        template<typename Archive>
        static inline void size(Archive &ar, Type &value, const Version &version) {
            ar += Type::header_size;
        }

        template<typename Archive>
        static inline void jump(Archive &ar, Type &value, const Version &version) {
            ar.get().seekg(Type::header_size, Archive::StreamType::cur);
        }

    };

    template<typename T, typename U>
    struct SaveLoadSize<HeadedData<T, U>> {
        using Type = HeadedData<T, U>;

        template<typename Archive>
        static inline void save(Archive &ar, Type &value, const Version &version) {
            serialize(ar, value.header, version);
            serialize(ar, value.data, version);
        }

        template<typename Archive>
        static inline void load(Archive &ar, Type &value, const Version &version) {
            serialize(ar, value.header, version);
            serialize(ar, value.data, version);
        }

        template<typename Archive>
        static inline void size(Archive &ar, Type &value, const Version &version) {
            auto new_ar = Archive();
            serialize(new_ar, value.data, version);
            value.header.data_size = new_ar.get();

            ar += T::header_size + new_ar;
        }

        template<typename Archive>
        static inline void jump(Archive &ar, Type &value, const Version &version) {
            load(ar, value, version);
        }

    };

    class SizeSerializer {
    private:
        DataSize size_{0};

    public:
        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::size(*this, other, {});
            return *this;
        }

        template<typename T>
        auto &operator>>(T &other) {
            auto version = descriptor_data<T>::version;

            serialize(*this, other, version);
            return *this;
        }

        [[nodiscard]] auto get() const noexcept {
            return size_;
        }

        auto clear() noexcept {
            size_ = 0;
        }

        template<typename T>
        SizeSerializer &operator+=(const T &other) {
            size_ += other;
            return *this;
        }

        template<typename T>
        SizeSerializer operator+(const T &other) {
            auto temp = size_ + other;
            return temp;
        }

        template<typename T>
        SizeSerializer &operator*=(const T &other) {
            size_ *= other;
            return *this;
        }

        template<typename T>
        SizeSerializer operator*(const T &other) {
            auto temp = *this;
            temp += other;
            return temp;
        }

        template<typename T>
        requires (!std::is_same_v<T, SizeSerializer>)
        friend T &operator+=(T &left, const SizeSerializer &right);

        template<typename T>
        requires (!std::is_same_v<T, SizeSerializer>)
        friend T operator+(T &left, const SizeSerializer &right);

        template<typename T>
        requires (!std::is_same_v<T, SizeSerializer>)
        friend T &operator*=(T &left, const SizeSerializer &right);

        template<typename T>
        requires (!std::is_same_v<T, SizeSerializer>)
        friend T operator*(T &left, const SizeSerializer &right);
    };

    template<typename T>
    requires (!std::is_same_v<T, SizeSerializer>)
    T &operator+=(T &left, const SizeSerializer &right) {
        left += right.size_;
        return left;
    }

    template<typename T>
    requires (!std::is_same_v<T, SizeSerializer>)
    T operator+(T &left, const SizeSerializer &right) {
        auto temp = left + right.size_;
        return temp;
    }

    template<typename T>
    requires (!std::is_same_v<T, SizeSerializer>)
    T &operator*=(T &left, const SizeSerializer &right) {
        left *= right.size_;
        return left;
    }

    template<typename T>
    requires (!std::is_same_v<T, SizeSerializer>)
    T operator*(T &left, const SizeSerializer &right) {
        auto temp = left*right.size_;
        return temp;
    }

}

#endif //PONG_SRC_PONG_SERIAL_DATA_HEADER_DATA_H_
