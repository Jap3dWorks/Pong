//
// Created by Jordi on 6/19/2022.
//

#ifndef GL_TEST_LOAD_MODEL_LEVEL_H
#define GL_TEST_LOAD_MODEL_LEVEL_H

#include "Pong/core/core_vals.h"
#include "Pong/core/level.h"
#include "Pong/core/material.h"
#include "Pong/core/lights.h"
#include "Pong/logger.h"
#include "Pong/shapes/cube_shape.h.h"
#include "Pong/default_materials.h"
#include "Levels/blending_level.h"
#include "Pong/shapes/plane_shape.h"
//#include "Pong/Shapes/skybox_shape.h"
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
        a_camera->Position = glm::vec3(0, 0, 9);

        // grass planes
        auto basic_shd = _scene->create_shader("basic_shd",
                                               "./Shaders/basic.glsl");
        auto basic_mat = _scene->create_material<Pong::Material>(
                "basic_mat",
                basic_shd,
                {});

        auto grass_shp = _scene->create_shape<Pong::GraphicMesh>("grass_shp");
        Pong::CubeShape(grass_shp->get_mesh()).build();

        auto grass_act_01 = _scene->create_actor<Pong::APlayer>("grass_act_01");

        grass_act_01->set_transform(glm::rotate(grass_act_01->get_transform(),
                                                (float)_P_PI,
                                                glm::vec3(0,0,1)));

        _scene->assign_material(basic_mat, grass_shp);
        _scene->assign_shape(grass_shp, grass_act_01);

        // a blinn material
        auto blinn_shd = _scene->create_shader("blinn_shd",
                                               "./Shaders/blinn.glsl");

        auto blinn_mat = _scene->create_material<Pong::Material>("blinn_mat",
                                                                 blinn_shd,
                                                                 {});

        blinn_mat->set_param("surfaceColor", glm::vec3(0.1, 0.2, 0.3));
        blinn_mat->set_param("glow", 16.f);
        blinn_mat->set_param("specular", .8f);

        // TODO: https://learnopengl.com/Advanced-OpenGL/Instancing

        // --lighting--
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
