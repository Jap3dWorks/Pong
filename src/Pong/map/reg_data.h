//
// Created by Jordi on 8/4/2022.
//

#ifndef GL_TEST_REG_DATA_H
#define GL_TEST_REG_DATA_H

#include "sparse_set.h"
#include "type_reg_map.h"
#include "reg_id_manager.h"
#include "Pong/components/component.h"
#include "Utils/type_conditions.h"
#include <tuple>
#include <vector>
#include <unordered_set>
#include <type_traits>
#include <concepts>

namespace Pong {
    /**
     * RegData represents conjunction of related classes data.
     * RegData manages each class separately using a SparseSets system
     * (TypeRegMap).
     * e.g all components should be included in the same RegData.
     * Also RegData use RegId for elements conjunction.
     * */
    template<typename ...Types>
    class RegData {
    private:
        using Registry = TypeRegMap;
        using id_set_t = RegIdManager<uint32_t>;

        template<Intersects<Types...> comp>
        using type_data = SparseSet<comp>;

    private:
        Registry registry_{};
        id_set_t reg_id_{};

    public:
        RegData() {
            (registry_.registry<Types>(), ...);
    }

        auto& get_registry() noexcept {
            return registry_;
        }

        auto& get_id_registry() noexcept {
            return reg_id_;
        }

        auto create_id() {
            return reg_id_.create_id();
        }

        template<Intersects<Types...> type_>
        constexpr auto& get_type(RegId id_) const {
            registry_.template get<type_data<type_>>().at(
                    to_integer(id_)
            );
        }

        template<Intersects<Types...> type_>
        auto &add_type(RegId id_) {
            return registry_.template get<type_data<type_>>().insert(
                    to_integer(id_), type_()
            );
        }

        template<Intersects<Types...> type_>
        constexpr auto& get_types() {
            return registry_.template get<type_data<type_>>();
        }
    };
}


#endif //GL_TEST_REG_DATA_H
