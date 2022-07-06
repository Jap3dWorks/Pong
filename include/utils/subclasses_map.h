//
// Created by Jordi on 7/2/2022.
//

#ifndef GL_TEST_SUBCLASSES_MAP_H
#define GL_TEST_SUBCLASSES_MAP_H
#include <map>
#include <vector>
#include <iostream>


template<typename _base>
class SubClassMap {
private:
    using BasePtr = _base*;
    using BaseVector = std::vector<BasePtr>;
    using SubclassData = std::map<size_t, BaseVector>;
    using BaseVectorIterator = typename BaseVector::iterator;

private:
    SubclassData _subclass_map;
    std::vector<std::unique_ptr<_base>> _storage;

public:
    SubClassMap()=default;
    ~SubClassMap()=default;

    SubClassMap(SubClassMap&& other) noexcept {
        std::swap(_subclass_map, other._subclass_map);
    }

    SubClassMap& operator=(SubClassMap&& other) noexcept {
        _subclass_map = std::move(other._subclass_map);
    }

    SubClassMap(const SubClassMap& other) = delete;
    SubClassMap& operator=(const SubClassMap& other) = delete;

public:
    // https://en.cppreference.com/w/cpp/language/constraints
    // Add conpect to family reg

    template<typename T>
    void register_type() {
        auto& t_info = typeid(T);
        auto type_hash = t_info.hash_code();
        assert(!_subclass_map.contains(t_info.hash_code()));
        _subclass_map[type_hash] = std::vector<_base*>();
    }

    template<typename T>
    bool registered() {
        auto& t_info = typeid(T);
        return _subclass_map.contains(t_info.hash_code());
    }

    template<typename T>
    void push_back(T* component) {
        auto& t_info = typeid(T);
        _subclass_map[t_info.hash_code()].push_back(component);
    }

    template<typename T>
    void push_back(T &&component) {
        auto _nw_ptr = std::make_unique<T>(std::forward<T>(component));
        auto _ptr = _nw_ptr.get();
        _storage.push_back(std::move(_nw_ptr));
        _subclass_map[typeid(T).hash_code()].push_back(_ptr);
    }

    template<typename T, class... Args>
    void emplace_back(Args&&... args) {
        auto _nw_ptr = std::make_unique<T>(std::forward<Args>(args)...);
        auto _ptr = _nw_ptr.get();
        _storage.push_back(std::move(_nw_ptr));
        _subclass_map[typeid(T).hash_code()].push_back(_ptr);
    }

    const SubclassData& data() {
        return _subclass_map;
    }

public:
    class iterator {
    private:
        using SubclassDataIterator = typename SubclassData::iterator;

    private:
        BasePtr* _ptr;
        BasePtr* _end;
        SubclassDataIterator _data_itr;
        SubclassDataIterator _end_data_itr;
        SubclassData* _address;

    public:
        BasePtr& operator*() {
            return *_ptr;
        }

        BasePtr* operator->() {
            return _ptr;
        }

        iterator(SubclassDataIterator &&data_itr, SubclassDataIterator &&end_data_itr,
                 SubclassData *address, BasePtr *ptr = nullptr) :
                _address(address), _data_itr(std::move(data_itr)),
                _end_data_itr(std::move(end_data_itr)) {
            if (ptr) {
                _ptr = ptr;
            } else {
                _ptr = itr_begin(_data_itr);
            }

            _end = itr_end(_data_itr);
        }

        static inline BasePtr* itr_begin(const SubclassDataIterator& data_itr) noexcept {
            return data_itr->second.data();
        }

        static inline BasePtr* itr_end(const SubclassDataIterator& data_itr) noexcept {
            return data_itr->second.data() + data_itr->second.size();
        }

        inline void _compat(const iterator& other) {
            assert(_address == other._address && "Iterators are not compatible!");
        }

        iterator& operator++() {
            ++_ptr;
            if (_ptr == _end) {
                ++_data_itr;
                if (_data_itr != _end_data_itr) {
                    _ptr = itr_begin(_data_itr);
                    _end = itr_end(_data_itr);
                }
            }

            return *this;
        }

        [[nodiscard]] iterator operator++(int) {
            auto _temp = *this;
            ++(*this);
            return _temp;
        }

        bool operator== (const iterator& b) const noexcept {
            assert(_address == b._address && "Iterators are not compatible!");
            return _ptr == b._ptr;
        }

        bool operator!= (const iterator& b) const noexcept {
            assert(_address == b._address && "Iterators are not compatible!");
            return _ptr != b._ptr;
        }

    };

    template<typename T>
    BaseVectorIterator begin() {
        auto _hash = typeid(T).hash_code();
        auto _map_iterator = _subclass_map.find(_hash);
        assert(_map_iterator != _subclass_map.end() && "SubType not registered!");

        return _map_iterator->second.begin();
    }

    template<typename T>
    BaseVectorIterator end() {
        auto _hash = typeid(T).hash_code();
        auto _map_iterator = _subclass_map.find(_hash);
        assert(_map_iterator != _subclass_map.end());

        return _map_iterator->second.end();
    }

    iterator begin() {
        return {_subclass_map.begin(),
                _subclass_map.end(),
                &_subclass_map};
    }

    iterator end() {
        auto _end_map_ptr = _subclass_map.end();
        --_end_map_ptr;
        auto _end = iterator::itr_end(_end_map_ptr);
        return {std::move(_end_map_ptr),
                _subclass_map.end(),
                &_subclass_map,
                _end};
    }
};

#endif //GL_TEST_SUBCLASSES_MAP_H
