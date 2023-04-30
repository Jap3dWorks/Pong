//
// Created by Jordi on 8/28/2022.
//

#ifndef PONG_SRC_PONG_ENGINE_ENGINE_REGISTERS_H_
#define PONG_SRC_PONG_ENGINE_ENGINE_REGISTERS_H_

#include "Pong/map/map.h"
#include "Pong/core/material.h"
#include "Pong/registers/reg_data_controller.h"
#include "Pong/components/component.h"
#include "Pong/components/map_component.h"
#include "Pong/core/geometry_data.h"
#include "Utils/type_conditions.h"


namespace pong::engine {



    struct buffered_data_t {};
    struct location_data_t {};
    struct map_data_t {};

    using GraphicBufferRegister = RegDataController<Mesh, Curve, Material>;
    using MapBufferRegister = RegDataController<pong::map::Map>;

    template<typename T=buffered_data_t>
    struct EngineDataRegister {
        using Type = T;
        GraphicBufferRegister graphic_reg{};
        MapBufferRegister map_reg{};
    };


    template<typename ...Args>
    struct RegDataLocation {
        RegId reg_id{};
        std::string file_path{};

        static constexpr inline uint32_t types_count{sizeof...(Args)};

        template<uint32_t N>
        using IndexType = variadic_id_t<N, Args...>;
    };


    using MeshDtLocation = RegDataLocation<Mesh>;
    using CurveDtLocation = RegDataLocation<Curve>;
    using MaterialDtLocation = RegDataLocation<Material>;
    using MapDtLocation = RegDataLocation<map::Map>;

    using GraphicRegisterLocation = RegDataController<MeshDtLocation,
                                                      CurveDtLocation,
                                                      MaterialDtLocation>;

    using MapRegisterLocation = RegDataController<MapDtLocation>;

    template<>
    struct EngineDataRegister<location_data_t> {
        using Type = location_data_t;

        GraphicRegisterLocation graphic_reg{};
        MapRegisterLocation map_reg{};

    };

    using BufferedRegisters = EngineDataRegister<buffered_data_t>;
    using LocationRegisters = EngineDataRegister<location_data_t>;

}


#endif //PONG_SRC_PONG_ENGINE_ENGINE_REGISTERS_H_
