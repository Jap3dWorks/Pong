//
// Created by Jordi on 8/29/2022.
//

#ifndef PONG_SRC_PONG_MAP_MAP_H_
#define PONG_SRC_PONG_MAP_MAP_H_

#include "Pong/registers/reg_id.h"
#include "Pong/registers/reg_data_controller.h"
#include "Utils/type_conditions.h"
#include <vector>

namespace pong::map {
    using EntityComponentsTypes = TypesStruct<component::TransformComponent,
                                              component::CameraComponent,
                                              component::StaticMeshComponent,
                                              component::CubemapComponent,
                                              component::PythonComponent>;

    using ComponentRegController = RegDataController<EntityComponentsTypes::get<0>::type,
                                                     EntityComponentsTypes::get<1>::type,
                                                     EntityComponentsTypes::get<2>::type,
                                                     EntityComponentsTypes::get<3>::type,
                                                     EntityComponentsTypes::get<4>::type
                                                     >;

    using MapComponentsTypes = TypesStruct<component::PythonComponent>;
    using MapComponentRegController = RegDataController<MapComponentsTypes::get<0>::type>;


    struct MapRegister {
        ComponentRegController component_reg{};
        MapComponentRegController map_component_reg{};
    };

    struct Map {
        RegId reg_id;
        MapRegister map_register{};
        std::vector<RegId> maps;
    };

}
#endif //PONG_SRC_PONG_MAP_MAP_H_
