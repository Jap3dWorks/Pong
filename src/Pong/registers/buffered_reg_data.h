//
// Created by Jordi on 8/4/2022.
//

#ifndef GL_TEST_REG_DATA_H
#define GL_TEST_REG_DATA_H

#include "sparse_set.h"
#include "type_reg_map.h"
#include "Pong/registers/reg_id.h"
#include "Pong/components/component.h"
#include "Utils/type_conditions.h"
#include <tuple>
#include <vector>
#include <unordered_set>
#include <type_traits>
#include <concepts>

namespace Pong {
    /**
     * BufferedRegData represents conjunction of related classes data.
     * BufferedRegData manages each class separately using a SparseSets system
     * (TypeRegMap).
     * e.g all components should be included in the same BufferedRegData.
     * Also BufferedRegData use RegId for elements conjunction.
     * BufferedRegData do not apply any restriction with ids parallelism.
     * This behaviour its ok with Components, but is the desired behaviour between shapes and meshes?.
     * */
    template<typename ...Types>
    class BufferedRegData {
    private:
        using Registry = TypeRegMap;
        using id_set_t = RegIdArray<>;

        template<Intersects<Types...> comp>
        using type_data = SparseSet<comp>;

    private:
        Registry registry_{};
        id_set_t reg_id_{};

    public:
        BufferedRegData() {
            (registry_.registry<Types>(), ...);
    }

        auto& get_registry() noexcept {
            return registry_;
        }

        auto& get_id_registry() noexcept {
            return reg_id_;
        }

        auto create_id() {
            return reg_id_.create();
        }

        template<Intersects<Types...> type_>
        constexpr auto& get_type(RegId id_) const {
            registry_.template get<type_data<type_>>().at(
                    id_
            );
        }

        template<Intersects<Types...> type_>
        auto &add_type(RegId id_) {
            return registry_.template get<type_data<type_>>().insert(
                    id_, type_()
            );
        }

        template<Intersects<Types...> type_>
        constexpr auto& get_types() {
            return registry_.template get<type_data<type_>>();
        }
    };
}


#endif //GL_TEST_REG_DATA_H
