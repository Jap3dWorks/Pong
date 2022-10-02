//
// Created by Jordi on 8/29/2022.
//

#ifndef PONG_SRC_PONG_MAP_MAP_H_
#define PONG_SRC_PONG_MAP_MAP_H_

#include "Pong/components/component.h"
#include "Pong/registers/reg_id.h"
#include "Pong/registers/reg_data_controller.h"

#include "Utils/type_conditions.h"
#include <vector>


namespace pong::map {

//    using EntityComponentsTypes = TypesStruct<
//        component::TransformComp,
//        component::CameraComp,
//        component::TransformComp,
//        component::CubemapComp,
//        component::PythonComp>;


// TODO: reflection here, update components
//    using EntityComponentRegController = RegDataController<
//        EntityComponentsTypes::get<0>::type,
//        EntityComponentsTypes::get<1>::type,
//        EntityComponentsTypes::get<2>::type,
//        EntityComponentsTypes::get<3>::type,
//        EntityComponentsTypes::get<4>::type
//    >;

    struct Map {
        RegId reg_id;
        RegDataController entity_reg{};
//        component::PythonComp python_component{};

        std::vector<RegId> maps;

    };

}

#endif //PONG_SRC_PONG_MAP_MAP_H_
