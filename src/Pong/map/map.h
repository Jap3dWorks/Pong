//
// Created by Jordi on 8/29/2022.
//

#ifndef PONG_SRC_PONG_MAP_MAP_H_
#define PONG_SRC_PONG_MAP_MAP_H_

#include "Pong/registers/reg_id.h"
#include "Pong/registers/reg_data_controller.h"
#include "Pong/engine/engine_registers.h"
#include <vector>

namespace pong::map {
    using ComponentRegController = RegDataController<component::TransformComponent,
                                                     component::CameraComponent,
                                                     component::StaticMeshComponent>;

    using MapComponentRegController = RegDataController<component::PythonComponent>;

    struct MapRegister {
        ComponentRegController component_reg{};
        MapComponentRegController map_component_reg{};
    };

    struct Map {
        RegId reg_id{0};
        MapRegister map_register{};
        std::vector<RegId> maps;
    };

}
#endif //PONG_SRC_PONG_MAP_MAP_H_
