//
// Created by Jordi on 8/8/2022.
//

#ifndef PONG_SRC_PONG_MAP_MAP_LOGIC_H_
#define PONG_SRC_PONG_MAP_MAP_LOGIC_H_

#include "Pong/registers/reg_data_controller.h"
#include "Pong/engine/engine.h"
#include "Pong/components/component.h"
#include "Pong/components/collision_components.h"
#include "Pong/core/geometry_data.h"
#include "Pong/render/render_queue.h"
#include "Pong/core/material.h"
#include <functional>


namespace pong::map {
/*
 * MapLogic (provisional name) should manage the current map actors and components
 * And apply changes to any component in the map.
 * Provisionaly MapLogic also has all the logic related with send jobs to renderer and
 * update map each frame.
 */

    class MapLogic {
    private:
        std::reference_wrapper<RenderQueueSubmitter> submitter_;

    public:

        virtual void setup() {}

        void start() {
            // auto& registry = entity_reg.get_reg_data();
            // registry.registry<>();
            // Build here views or gruops, e.g staticMesh with transp materials and transforms
            // staticMesh with opaque materials and transforms

        }

        void load() {
        }

        void save() {
        }

        // every frame logic
        void update() {
            // Run logics and submit to render.

        }

//        void update_meshes() {
//            for (auto& mshcmp: entity_reg.get_types<StaticMeshComponent>()) {
//                 do things
//            }
//        }


//        auto& get_mesh(RegId id_) {

//            return mesh_reg.get_type<>(id_);
//        }

    };
}

#endif //PONG_SRC_PONG_MAP_MAP_LOGIC_H_
