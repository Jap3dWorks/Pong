//
// Created by Jordi on 8/8/2022.
//

#ifndef GL_TEST_REG_ID_SET_H
#define GL_TEST_REG_ID_SET_H
#include<iostream>
#include<unordered_set>
#include <queue>

template<typename IdType=uint32_t>
class RegIdSet {
public:
    using iterator = typename std::unordered_set<IdType>::iterator;

private:
    std::unordered_set<IdType> reg_id_;
    std::queue<IdType> free_index_;

    IdType last_id_{0};

public:
    auto create_id() {
        if (free_index_.empty()) {
            reg_id_.insert(++last_id_);
            return last_id_;
        }
        else {
            auto _front = free_index_.front();
            free_index_.pop();
            reg_id_.insert(_front);
            return _front;
        }
    }

    inline auto begin() {
        return reg_id_.begin();
    }

    inline auto end() {
        return reg_id_.end();
    }

    auto erase(IdType id_) {
        auto removed = reg_id_.remove(id_);
        free_index_.push(id_);
        return removed;
    }
};


#endif //GL_TEST_REG_ID_SET_H
