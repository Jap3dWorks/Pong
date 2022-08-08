//
// Created by Jordi on 8/8/2022.
//

#ifndef GL_TEST_MAP_LEVEL_H
#define GL_TEST_MAP_LEVEL_H

#include "Pong/core/map/map_reg.h"
#include "Pong/components/camera_component.h"
#include "Pong/components/collision_components.h"
#include "Pong/components/graphic_component.h"

//#include "Pong/core/map/"
//#include "Pong/core/map/reg_id_set.h"



namespace Pong {
    class MapLevel {

    public:
        RegData<TransformComponent,
                CameraComponent,
                StaticMeshComponent> components_reg{};

        RegData<> mesh_reg{};
        RegData<> material_reg{};

    public:

        virtual void setup() {

        }

        void start() {
            auto& registry = components_reg.get_registry();
//            registry.registry<>();
                // Build here views or gruops, e.g staticMesh with transp materials and transforms
                // staticMesh with opaque materials and transforms

        }

        void update() {

        }

        void update_meshes() {
//            auto transform_comps =

            for (auto& mshcmp: components_reg.get_types<StaticMeshComponent>()) {
                // do things
            }
        }
    };
}

#endif //GL_TEST_MAP_LEVEL_H
