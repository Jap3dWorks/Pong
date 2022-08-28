//
// Created by Jordi on 8/28/2022.
//

#ifndef PONG_SRC_PONG_ENGINE_ENGINE_REGISTERS_H_
#define PONG_SRC_PONG_ENGINE_ENGINE_REGISTERS_H_

#include "Pong/registers/reg_data_controller.h"
#include "Pong/components/component.h"
#include "Pong/core/geometry_data.h"
#include "Pong/core/material.h"


namespace pong::engine {

    struct buffered_data_t {};
    struct location_data_t {};
    struct map_data_t {};

    template<typename T=buffered_data_t>
    struct EngineDataRegister {
        using type = T;

        RegDataController<Mesh, Curve, Material> graphic_reg{};
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

    using GraphicRegisterLocation = RegDataController<MeshDtLocation,
                                                      CurveDtLocation,
                                                      MaterialDtLocation>;


    template<>
    struct EngineDataRegister<location_data_t> {
        using type = location_data_t;

        GraphicRegisterLocation graphic_reg{};
    };

    using BufferedRegister = EngineDataRegister<buffered_data_t>;
    using LocationRegister = EngineDataRegister<location_data_t>;


    using ComponentDataController = RegDataController<TransformComponent,
                                                      CameraComponent,
                                                      StaticMeshComponent>;
    template<>
    struct EngineDataRegister<map_data_t> {
        using type = map_data_t;

        ComponentDataController component_reg{};

    };

    using MapRegister = EngineDataRegister<map_data_t>;

}


#endif //PONG_SRC_PONG_ENGINE_ENGINE_REGISTERS_H_
