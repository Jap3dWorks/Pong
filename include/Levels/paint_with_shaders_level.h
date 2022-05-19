//
// Created by Jordi on 4/24/2022.
//

#ifndef PONG_PAINT_WITH_SHADERS_LEVEL_H
#define PONG_PAINT_WITH_SHADERS_LEVEL_H

#include "Pong/Core/core_vals.h"
#include "Pong/Core/level.h"
#include "Pong/Core/material.h"
#include "Pong/Core/lights.h"
#include "Pong/logger.h"
#include "Pong/default_materials.h"
#include "Levels/blending_level.h"
#include <vector>

// TODO: quad level for Shader testing
class PaintWithShades : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        AbstractLevel::_level_setup();

        // add a camera quad
        auto draw_shader = _scene->create_shader(
                "draw_shd",
                "../Shaders/raymarching_shaders/primitives_scene.glsl");

        auto draw_material =
                _scene->create_material<Pong::Material>(
                "draw_mat", draw_shader,
                {});

        // full screen quad
        auto draw_shape = _scene->create_shape<Pong::PlaneShape>(
                "draw_shape", 2.0, 2.0);

        auto draw_actor = _scene->create_actor<Pong::APlayer>("Draw Actor");

//        draw_actor->set_transform({1,0,0,0,
//                                   0,1,0,0,
//                                   0,0,1,0,
//                                   0,0,0,1});

        _scene->get_camera()->Position = glm::vec3(0.0, 5.0, 5.0);
//        _scene->get_camera()->set_transform({1,0,0,0,
//                                             0,1,0,0,
//                                             0,0,1,0,
//                                             0,0,-5,1});
        _scene->assign_material(draw_material, draw_shape);
        _scene->assign_shape(draw_shape, draw_actor);
    }
};

#endif //PONG_PAINT_WITH_SHADERS_LEVEL_H
