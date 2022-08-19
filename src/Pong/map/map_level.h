//
// Created by Jordi on 8/8/2022.
//

#ifndef GL_TEST_MAP_LEVEL_H
#define GL_TEST_MAP_LEVEL_H

#include "Pong/registers/buffered_reg_data.h"
#include "Pong/components/component.h"
#include "Pong/components/collision_components.h"
#include "Pong/core/geometry_data.h"
#include "Pong/render/render_queue.h"
#include "Pong/core/material.h"
#include <functional>


namespace Pong {
/*
 * MapLevel (provisional name) should manage the current map actors and components
 * And apply changes to any component in the scene.
 * Provisionaly MapLevel also has all the logic related with send jobs to renderer and
 * update scene each frame.
 */
    class MapLevel {
    private:
        std::reference_wrapper<RenderQueueSubmitter> submitter;

    public:
        BufferedRegData<TransformComponent,
                CameraComponent,
                StaticMeshComponent> components_reg{};

        BufferedRegData<Mesh, Curve> shape_reg{}; // a temp buffer with geo buffers
        BufferedRegData<Material> material_reg{};  // a temp buffer with mat buffers

    public:

        virtual void setup() {}

        void start() {
            auto& registry = components_reg.get_registry();
//            registry.registry<>();
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

        void update_meshes() {
            for (auto& mshcmp: components_reg.get_types<StaticMeshComponent>()) {
                // do things
            }
        }


//        auto& get_mesh(RegId id_) {

//            return mesh_reg.get_type<>(id_);
//        }

    };
}

#endif //GL_TEST_MAP_LEVEL_H
