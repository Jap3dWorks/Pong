//
// Created by Jordi on 7/5/2022.
//

#ifndef GL_TEST_CONTINUOUS_STORAGE_H
#define GL_TEST_CONTINUOUS_STORAGE_H

#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <string>

/*
 * Storage objects must be derived from BaseClass */

template<typename BaseClass>
class BufferStorage {
public:
    using StoragePtr = uint8_t*;

private:
    inline static constexpr uint32_t _max=64;  // 512 is a lot of storage

    std::vector<BaseClass*> storage_objects{};
    uint32_t quantities[_max]{};
    StoragePtr datas[_max]{};
    uint32_t datas_num=0;
    uint32_t min_size = 32;  // initial storage size

public:
    BufferStorage() {
        datas[0] = new uint8_t[min_size];
        datas_num = 1;
    };

    ~BufferStorage() {
        for (auto& ptr: storage_objects) {
            delete ptr;
        }

        for (uint32_t i=0; i<datas_num; ++i) {
            delete[] datas[i];
        }
    }

private:
    [[nodiscard]] inline uint32_t data_size(uint32_t _data_num) const {
        return min_size * std::pow(2, _data_num);
    }

    inline uint32_t valid_array_id(uint32_t size) {
        for (uint32_t i=0; i < datas_num; ++i) {
            if (size <= (data_size(i) - quantities[i])) {
                return i;
            }
        }

        quantities[datas_num] = 0;
        datas[datas_num] = new uint8_t[data_size(datas_num)];
        return datas_num++;
    }

    template<typename T>
    inline void ensure_base() {
#ifndef NDEBUG
        auto value = std::is_base_of_v<BaseClass, T>;
        assert(value && "Invalid Derived Class!");
#endif
    }

public:
    template<typename T>
    auto insert(const T &data) -> T* {
        ensure_base<T>();

        auto valid_data = valid_array_id(sizeof(T));
        auto ptr = datas[valid_data] + quantities[valid_data];
        new((T*) ptr) T(data);

        storage_objects.push_back((BaseClass*) ptr);
        quantities[valid_data] += sizeof(T);
        return (T*) ptr;
    }

    template<typename T,
            class=typename std::enable_if<!std::is_lvalue_reference<T>::value>::type>
    auto insert(T &&data) -> T* {
        ensure_base<T>();

        auto data_num = valid_array_id(sizeof(T));
        auto ptr = datas[data_num] + quantities[data_num];
        new((T*)ptr) T(std::forward<T>(data));

        storage_objects.push_back((BaseClass*) ptr);
        quantities[data_num] += sizeof(T);
        return (T*) ptr;
    }

    template<typename T, typename ...Args>
    auto emplace(Args ...args) -> T* {
        ensure_base<T>();

        auto data_num = valid_array_id(sizeof(T));
        auto ptr = datas[data_num] + quantities[data_num];
        new((T*) ptr) T(args...);

        storage_objects.push_back((BaseClass*) ptr);
        quantities[data_num] += sizeof(T);
        return (T*) ptr;
    }

    class iterator {};

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
