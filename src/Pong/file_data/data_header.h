//
// Created by Jordi on 8/21/2022.
//

#ifndef PONG_DATA_HEADER_H
#define PONG_DATA_HEADER_H

#include "Pong/file_data/reflectable.h"
#include "Pong/file_data/serialize_functions.h"
#include "Pong/file_data/serialize_types.h"
#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>

namespace Pong::serializer {
    struct FileHeader_t{};

    template<typename T>
    struct data_header_ {
        static inline constexpr data_size_t header_size {
            sizeof(RegId) +
            sizeof(data_size_t)
        };

        SERIALIZABLE (
                FIELD(RegId, reg_id, 0),
                FIELD(data_size_t, data_size, 0)
        )

        using type = T;
    };

    template<>
    struct data_header_<FileHeader_t> {
        static inline constexpr data_size_t header_size {
            (sizeof(data_size_t) * 2) + P_MAX_SERIALIZER_NAME_LENGTH};

        SERIALIZABLE(
        FIELD(data_size_t, data_size, 0),
        FIELD(Version, version),
        FIELD(const char, type_name[P_MAX_SERIALIZER_NAME_LENGTH])
        )

        using type = FileHeader_t;
    };

    template<typename T>
    using Header = data_header_<T>;
    using FileHeader = data_header_<FileHeader_t>;

    template<typename Archive, typename T>
    void serialize(Archive &ar, Header<T>& value, const Version &version) {
        serialize_fields(ar, value);
    }

    IMPL_SERIALIZE(FileHeader);


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
        using type = Header<T>;

        template<typename Archive>
        static inline void save(Archive& ar, type& value, const Version& version) {
            serialize(ar, value, version);
        }

        template<typename Archive>
        static inline void load(Archive& ar, type& value, const Version& version) {
            serialize(ar, value, version);
        }

        template<typename Archive>
        static inline void size(Archive& ar, type& value, const Version& version) {
            ar += type::header_size;
        }
    };

    template<typename T, typename U>
    struct SaveLoadSize<HeadedData<T, U>> {
        using type = HeadedData<T, U>;

        template<typename Archive>
        static inline void save(Archive& ar, type& value, const Version& version) {
            serialize(ar, value.header, version);
            serialize(ar, value.data, version);
        }

        template<typename Archive>
        static inline void load(Archive& ar, type& value, const Version& version) {
            serialize(ar, value.header, version);
            serialize(ar, value.data, version);
        }

        template<typename Archive>
        static inline void size(Archive& ar, type& value, const Version& version) {
            auto new_ar = Archive();
            serialize(new_ar, value.data, version);
            value.header.data_size = new_ar.get();

            ar += T::header_size + new_ar;
        }
    };


    class SizeSerializer {
    private:
        data_size_t size_{0};

    public:
        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::size(*this, other, {});
            return *this;
        }

        template<typename T>
        auto& operator>>(T &other) {
            auto version = descriptor_data<T>::version;
            strncpy((char *) other.data.header.type_name,
                    descriptor_data<T>::type,
                    std::min(strlen(descriptor_data<T>::type),
                             {P_MAX_SERIALIZER_NAME_LENGTH}));

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
        requires(!std::is_same_v<T, SizeSerializer>)
        friend T& operator+=(T& left_, const SizeSerializer& right_);

        template<typename T>
        requires(!std::is_same_v<T, SizeSerializer>)
        friend T operator+(T& left_, const SizeSerializer& right_);

        template<typename T>
        requires(!std::is_same_v<T, SizeSerializer>)
        friend T& operator*=(T& left_, const SizeSerializer& right_);

        template<typename T>
        requires(!std::is_same_v<T, SizeSerializer>)
        friend T operator*(T& left_, const SizeSerializer& right_);
    };

    template<typename T>
    requires(!std::is_same_v<T, SizeSerializer>)
    T &operator+=(T& left_, const SizeSerializer& right_) {
        left_ += right_.size_;
        return left_;
    }

    template<typename T>
    requires(!std::is_same_v<T, SizeSerializer>)
    T operator+(T& left_, const SizeSerializer& right_) {
        auto temp = left_ + right_.size_;
        return temp;
    }

    template<typename T>
    requires(!std::is_same_v<T, SizeSerializer>)
    T& operator*=(T& left_, const SizeSerializer& right_) {
        left_ *= right_.size_;
        return left_;
    }

    template<typename T>
    requires(!std::is_same_v<T, SizeSerializer>)
    T operator*(T& left_, const SizeSerializer& right_) {
        auto temp = left_ * right_.size_;
        return temp;
    }
}

#endif //PONG_DATA_HEADER_H
