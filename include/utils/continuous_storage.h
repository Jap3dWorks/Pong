//
// Created by Jordi on 7/5/2022.
//

#ifndef GL_TEST_CONTINUOUS_STORAGE_H
#define GL_TEST_CONTINUOUS_STORAGE_H

#include<map>
#include<iostream>
#include<vector>
#include<cmath>

/*
 * Doesn't relocate memory*/
class BufferStorage {
public:
    using StoragePtr = uint8_t*;

private:
    inline static constexpr uint32_t _max=512;

    uint32_t quantities[_max]{};
    uint8_t* datas[_max]{};
    uint32_t datas_num=0;
    uint32_t min_size = 16;

public:
    BufferStorage() {
        datas[0] = new uint8_t[min_size];
        datas_num = 1;
    };

    ~BufferStorage() {
        for (uint32_t i=0; i<datas_num; ++i) {
            delete[] datas[i];
        }
    }

private:
    [[nodiscard]] inline uint32_t data_num_size(uint32_t _data_num) const {
        return min_size * std::pow(2, _data_num);
    }

    inline uint32_t valid_array(uint32_t size) {
        for (uint32_t i=0; i < datas_num; ++i) {
            if (size <= (data_num_size(i) - quantities[i])) {
                return i;
            }
        }

        quantities[datas_num] = 0;
        datas[datas_num] = new uint8_t[data_num_size(datas_num)];
        return datas_num++;
    }

public:
    template<typename T>
    auto insert(const T &data) -> StoragePtr {
        auto valid_data = valid_array(sizeof(T));
        auto vdata = (StoragePtr) (&data);

        auto ptr = datas[valid_data] + quantities[valid_data];

        memcpy((void *) ptr,
               (void *) vdata,
               sizeof(T));

        quantities[valid_data] += sizeof(T);
        return ptr;
    }

    template<typename T,
            class=typename std::enable_if<!std::is_lvalue_reference<T>::value>::type>
    auto insert(T &&data) -> StoragePtr {
        auto data_num = valid_array(sizeof(T));
        auto vdata = (StoragePtr) (&data);

        auto ptr = datas[data_num] + quantities[data_num];

        memmove((void *) ptr,
                (void *) vdata,
                sizeof(T));

        quantities[data_num] += sizeof(T);
        return ptr;
    }

//    StoragePtr data() noexcept {
//        return _storage.data();
//    }
//
//    auto begin() noexcept {
//        return _storage.begin();
//    }
//
//    auto end() noexcept {
//        return _storage.end();
//    }
};

#endif //GL_TEST_CONTINUOUS_STORAGE_H
