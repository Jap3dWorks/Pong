//
// Created by Jordi on 8/4/2022.
//

#ifndef GL_TEST_MAP_REG_H
#define GL_TEST_MAP_REG_H

#include "Pong/core/map/sparse_set.h"
#include "Pong/core/map/type_reg_map.h"
#include "Pong/core/map/reg_id_set.h"
#include "Pong/components/component.h"
#include <tuple>
#include <vector>
#include <unordered_set>
#include <type_traits>
#include <concepts>

namespace Pong {

    enum class RegId : uint32_t {
    };

    constexpr auto to_integer(RegId reg_id) noexcept {
        return static_cast<std::underlying_type_t<RegId>>(reg_id);
    }

    // TODO: create views
    /**Abstraction layer for Actors as regIds*/
    template<typename ...Types>
    class RegData {
    private:
        using Registry = TypeRegMap;
        using map_data_t = RegData<Registry>;
        using id_set_t = RegIdSet<uint32_t>;

        // using valid_types = Args
        class check_class : Types... {};

        template<std::derived_from<Component> comp>
        using type_data = SparseSet<comp>;

    private:
        Registry registry_{};
        id_set_t reg_id_{};

    private:

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
            return RegId(reg_id_.create_id());
        }

        template<std::common_with<check_class> type_>
        constexpr auto& get_type(RegId id_) const {
            registry_.template get<type_data<type_>>().at(
                    to_integer(id_)
            );
        }

        template<std::common_with<check_class> type_>
        auto& add_type(RegId id_) {
            return registry_.template get<type_data<type_>>().insert(
                    to_integer(id_), type_{}
                    );
        }

        template<std::common_with<check_class> type_>
        constexpr auto& get_types() {
            return registry_.template get<type_data<type_>>();
        }
    };
}


#endif //GL_TEST_MAP_REG_H
