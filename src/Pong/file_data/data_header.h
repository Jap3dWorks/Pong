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


namespace Pong::serializer {
    struct FileHeader_t{};

    template<typename T>
    struct data_header_ {
        static inline constexpr data_size_t header_size {sizeof(data_size_t)};

        SERIALIZABLE (
        FIELD(data_size_t, data_size)
        )

        using type = T;
    };

    template<>
    struct data_header_<FileHeader_t> {
        static inline constexpr data_size_t header_size {(sizeof(data_size_t) * 2) +
            P_MAX_SERIALIZER_NAME_LENGTH};

        SERIALIZABLE(
        FIELD(data_size_t, data_size),
        FIELD(data_size_t, version),
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
        Header_ header;
        Data_ data;
    };

    template<typename Header_, typename Data_>
    struct HeadedData<Header_, Data_, reference_data_t> {
        Header_ header;
        Data_ &data;
    };

    template<typename Archive, typename Header, typename Data>
    void serialize(Archive &ar, HeadedData<Header, Data> &value, const Version &version) {
        serialize(ar, value.header, version);
        ar & value.data;
    }

    class SizeSerializer {
    private:
        data_size_t size_{0};

    public:
        template<typename T>
        auto &operator&(T &other) {
            SaveLoadSize<T>::size(*this, other, {});
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


    template<typename T>
    struct SaveLoadSize<Header<T>> {
        using type = Header<T>;

        template<typename Archive>
        static inline void save(Archive& ar, type& value, const Version& version) {
            ar.get().write((char*)&value, type::header_size);
        }

        template<typename Archive>
        static inline void load(Archive& ar, type& value, const Version& version) {
            ar.get().read((char*)&value, type::header_size);
        }

        template<typename Archive>
        static inline void size(Archive& ar, type& value, const Version& version) {
            ar += type::header_size;
        }
    };


    template<typename Header, typename Data>
    void set_headed_size(HeadedData<Header, Data> &headed_data) {
        auto size_ser = SizeSerializer();
        serialize(size_ser, headed_data.data);

        headed_data.header.data_size = size_ser.get();
    }

}

#endif //PONG_DATA_HEADER_H
