//
// Created by Jordi on 6/19/2022.
//

#ifndef GL_TEST_LOAD_MODEL_LEVEL_H
#define GL_TEST_LOAD_MODEL_LEVEL_H

#include "../Pong/core/core_vals.h"
#include "../Pong/core/level.h"
#include "../Pong/core/material.h"
#include "../Pong/core/lights.h"
#include "../utils/logger.h"
#include "Pong/shapes/cube_shape.h"
#include "../Pong/material_lib.h"
#include "Pong/shapes/plane_shape.h"
#include <vector>

// Load a model scene
class LoadModelLevel : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        super::_level_setup();

        // Enable GL_PROGRAM_POINT_SIZE
        _render->gl_enables_config |= 8;
        _render->update_enables();

        // get camera_ptr
        Pong::ACamera *a_camera = _scene->get_camera();
        a_camera->position = glm::vec3(0, 0, 9);

        // grass planes
        auto basic_shd = _scene->create_shader("basic_shd",
                                               "./src/shaders/basic.glsl");

        auto basic_mat = _scene->create_material<Pong::Material>(
                "basic_mat",
                Pong::RenderLayer::BASE,
                basic_shd);

        auto grass_shp = _scene->create_shape<Pong::StaticMeshComponent>("grass_shp");
        Pong::CubeShape(grass_shp->mesh).build();
        auto grass_act_01 = _scene->create_actor<Pong::APlayer>("grass_act_01");
        _scene->assign_material(basic_mat, grass_shp);
        _scene->assign_shape(grass_shp, grass_act_01);

        // a blinn material
         TODO:

        // --lighting--
        _set_lights();
    }

    void _set_lights() {
        auto& directional_light = _scene->get_directional_lights();
        directional_light[0].direction =
                glm::normalize(glm::vec4{0.3f, -1.f, -0.5f, 1.f});
        directional_light[0].color =
                glm::vec4{0.8f, 0.8f, 0.3f, 1.f};
        directional_light[0].ambient =
                glm::vec4{0.22f, 0.22f, 0.7f, 1.f};
    }
};

#endif //GL_TEST_LOAD_MODEL_LEVEL_H
