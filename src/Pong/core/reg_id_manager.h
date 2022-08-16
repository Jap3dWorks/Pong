//
// Created by Jordi on 8/8/2022.
//

#ifndef GL_TEST_REG_ID_MANAGER_H
#define GL_TEST_REG_ID_MANAGER_H

#include<iostream>
#include<unordered_set>
#include <queue>
#include <cassert>


namespace Pong {

    enum class RegId : uint32_t {
    };

    constexpr auto to_integer(RegId reg_id) noexcept {
        return static_cast<std::underlying_type_t<RegId>>(reg_id);
    }

    RegId &operator++(RegId &reg_id) {
        using IntType = typename std::underlying_type<RegId>::type;
        reg_id = static_cast<RegId>( static_cast<IntType>(reg_id) + 1 );
        return reg_id;
    }

    RegId operator++(RegId &c, int) {
        RegId result = c;
        ++c;
        return result;
    }

/**
 * RegIdManager manages the id creation, you can request for unused ids.
 * Also you can iterate over all created ids.
 * */
    template<typename IdType=RegId>
    class RegIdManager {
    public:
        using iterator = typename std::unordered_set<IdType>::iterator;

    private:
        std::unordered_set<IdType> reg_id_;
        std::queue<IdType> free_index_;

        IdType last_id_{0};

    public:
        IdType create_id() {
            if (free_index_.empty()) {
                reg_id_.insert(++last_id_);
                return {last_id_};
            } else {
                auto _front = free_index_.front();
                free_index_.pop();
                reg_id_.insert(_front);
                return {_front};
            }
        }

        /**
         * Register an already existent RegId
         * */
        auto register_id(IdType id_) {
            assert(!reg_id_.contains(id_));

            reg_id_.insert(id_);
        }

        bool contains(IdType id_) const {
            return reg_id_.contains(id_);
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

}
#endif //GL_TEST_REG_ID_MANAGER_H
