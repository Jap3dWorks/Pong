//
// Created by Jordi on 8/4/2022.
//

#ifndef PONG_SRC_PONG_REGISTERS_REG_DATA_CONTROLLER_H_
#define PONG_SRC_PONG_REGISTERS_REG_DATA_CONTROLLER_H_

#include "sparse_set.h"
#include "Pong/core/parameter_map.h"
#include "Pong/registers/reg_id.h"
#include "Pong/components/component.h"
#include "Utils/type_conditions.h"
#include <tuple>
#include <vector>
#include <unordered_set>
#include <type_traits>
#include <concepts>

namespace pong {

    /**
     * RegDataController represents conjunction of related classes data.
     * RegDataController manages each class separately using a SparseSets system
     * (ParameterMap).
     * e.g all components should be included in the same RegDataController.
     * Also RegDataController use RegId for elements conjunction.
     * RegDataController do not apply any restriction with ids parallelism.
     * This behaviour its ok with Components, but is the desired behaviour between shapes and meshes?.
     * */
    template<typename ...Types>
    class RegDataController {
    private:
        using reg_data_t = ParameterMap;
        using id_array_t = RegIdArray<>;

        template<Intersects<Types...> comp>
        using data_t = SparseSet<comp>;

    private:
        reg_data_t reg_data_{};
        id_array_t id_array_{};

    public:
        RegDataController() {
            (reg_data_.registry<Types>(), ...);
    }

        auto& get_reg_data() noexcept {
            return reg_data_;
        }

        auto& get_id_array() noexcept {
            return id_array_;
        }

        /**
         * Returns an unused id
         */
        auto create_id() {
            return id_array_.create();
        }

        template<Intersects<Types...> type_>
        auto erase_type(RegId reg_id) {
            if (id_array_.contains(reg_id)) {
                id_array_.erase(reg_id);
            }

            reg_data_.template get<data_t<type_>>().erase(
                    reg_id
            );
        }

        template<Intersects<Types...> type_>
        constexpr const auto& get_type(RegId reg_id) const {
            return reg_data_.template get<data_t<type_>>().at(
                    reg_id
            );
        }

        template<Intersects<Types...> type_>
        auto& get_type(RegId reg_id) {
            return reg_data_.template get<data_t<type_>>().at(
                reg_id
            );
        }

        template<Intersects<Types...> type_>
        auto insert_type(RegId reg_id, type_&& object_) {
            if (!id_array_.contains(reg_id)) {
                id_array_.insert(reg_id);
            }

            reg_data_.template get<data_t<type_>>().insert(
                reg_id, std::forward<type_>(object_)
            );
        }

        template<Intersects<Types...> type_>
        auto insert_type(RegId reg_id, const type_ &object_) {
            if (!id_array_.contains(reg_id)) {
                id_array_.insert(reg_id);
            }

            reg_data_.template get<data_t<type_>>().insert(
                reg_id, object_
            );
        }

        template<Intersects<Types...> type_>
        constexpr auto& get_types() {
            return reg_data_.template get<data_t<type_>>();
        }
    };
}


#endif //PONG_SRC_PONG_REGISTERS_REG_DATA_CONTROLLER_H_
