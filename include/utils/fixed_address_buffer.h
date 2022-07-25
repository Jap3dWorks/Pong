//
// Created by Jordi on 7/5/2022.
//

#ifndef FIXED_ADDRESS_BUFFER_H
#define FIXED_ADDRESS_BUFFER_H

#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <string>
#include "utils/type_conditions.h"

/**
 * Storage objects must be derived from BaseClass
 * FixedAddressBuffer ensures that memory address of all stored elements in
 * Buffer never will be moved to another memory address, so pointers to each
 * Buffer element will be consistent while FixedAddressBuffer object exists.
 */

template<typename BaseClass>
class FixedAddressBuffer {
public:
    using StoragePtr = uint8_t*;

private:
    inline static const uint32_t _max=64;  // 512 is a lot of storage

    std::vector<BaseClass*> _storage_objects{};
    uint32_t _quantities[_max]{};
    StoragePtr _datas[_max]{};
    uint32_t _datas_num=0;
    uint32_t _min_size = 32;  // initial storage size

public:
    FixedAddressBuffer() {
        _datas[0] = new uint8_t[_min_size];
        _datas_num = 1;
    }

    ~FixedAddressBuffer() {
        for (auto& ptr: _storage_objects) {
            ptr->~BaseClass();
        }

        for (uint32_t i=0; i < _datas_num; ++i) {
            delete[] _datas[i];
        }
    }

private:
    [[nodiscard]] inline uint32_t data_size(uint32_t _data_num) const {
        return _min_size * std::pow(2, _data_num);
    }

    inline uint32_t valid_array_id(uint32_t size) {
        for (uint32_t i=0; i < _datas_num; ++i) {
            if (size <= (data_size(i) - _quantities[i])) {
                return i;
            }
        }

        _quantities[_datas_num] = 0;
        _datas[_datas_num] = new uint8_t[data_size(_datas_num)];
        return _datas_num++;
    }

public:
    template<CondIsBase<BaseClass> T>
    auto insert(const T &data) -> T* {
        auto valid_data = valid_array_id(sizeof(T));
        auto ptr = _datas[valid_data] + _quantities[valid_data];
        new((T*) ptr) T(data);

        _storage_objects.push_back((BaseClass*) ptr);
        _quantities[valid_data] += sizeof(T);
        return (T*) ptr;
    }

    template<CondIsBase<BaseClass> T,
            class=ENSURE_LVALUE>
//    typename std::enable_if<std::is_base_of<Base, T>::value, ReturnType>::type
    auto insert(T &&data) -> T* {
        auto data_num = valid_array_id(sizeof(T));
        auto ptr = _datas[data_num] + _quantities[data_num];
        new((T*)ptr) T(std::forward<T>(data));

        _storage_objects.push_back((BaseClass*) ptr);
        _quantities[data_num] += sizeof(T);
        return (T*) ptr;
    }

    template<CondIsBase<BaseClass> T, typename ...Args>
    auto emplace(Args&& ...args) -> T* {
        auto data_num = valid_array_id(sizeof(T));
        auto ptr = _datas[data_num] + _quantities[data_num];
        new((T*) ptr) T(std::forward<Args>(args)...);

        _storage_objects.push_back((BaseClass*) ptr);
        _quantities[data_num] += sizeof(T);
        return (T*) ptr;
    }

//    class iterator {};

//    StoragePtr data() noexcept {
//        return _storage.data();
//    }

//    auto begin() noexcept {
//        return _storage.begin();
//    }

//    auto end() noexcept {
//        return _storage.end();
//    }

};

#endif // FIXED_ADDRESS_BUFFER_H
