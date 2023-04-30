//
// Created by Jordi on 8/21/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_HEADER_DATA_H_
#define PONG_SRC_PONG_SERIALIZER_HEADER_DATA_H_

#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>

#include "Pong/serializer/serial_types.h"
#include "Pong/registers/reg_id.h"
#include "Pong/config/config.h"


namespace pong::serializer {

    struct FileHeader_t {};

    template<typename T>
    struct data_header_ {
        static inline constexpr DataSize header_size{
            sizeof(RegId) +
                sizeof(DataSize)
        };

        RegId reg_id{0};
        DataSize data_size{0};

        using Type = T;
    };

    template<>
    struct data_header_<FileHeader_t> {
        static constexpr DataSize header_size{
            sizeof(RegId) +
            (sizeof(DataSize)*2) +
            P_MAX_SERIALIZER_NAME_LENGTH};

        RegId reg_id{0};
        DataSize data_size{0};
        Version version{};
        const char type_name[P_MAX_SERIALIZER_NAME_LENGTH]{};

        using Type = FileHeader_t;
    };

    using FileHeader = data_header_<FileHeader_t>;

    template<typename T>
    using Header = data_header_<T>;

    template<typename Archive, typename T>
    void serialize(Archive &ar, Header<T> &value, const Version &version) {
        ar & value.reg_id;
        ar & value.data_size;
    }

    template<typename Archive>
    void serialize(Archive &ar, FileHeader &value, const Version &version) {
        ar & value.reg_id;
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


}

#endif //PONG_SRC_PONG_SERIALIZER_HEADER_DATA_H_
