//
// Created by Jordi on 8/4/2022.
//

#ifndef GL_TEST_MAP_REG_H
#define GL_TEST_MAP_REG_H

#include "Pong/core/map/map_sset.h"
#include "Pong/core/map/type_reg.h"
#include <tuple>

namespace Pong {

    enum class RegId : uint32_t {
    };

    constexpr auto to_integer(RegId reg_id) noexcept {
        return static_cast<std::underlying_type_t<RegId>>(reg_id);
    }

    template<typename ...Args>
    class MapReg {
    private:
        TypeRegMap type_reg_{};

    public:
        MapReg() {
            // fold expression
            // https://en.cppreference.com/w/cpp/language/fold
            (type_reg_.template registry<Args>(), ...);
        }
    };
}


#endif //GL_TEST_MAP_REG_H
