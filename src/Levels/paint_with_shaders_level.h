//
// Created by Jordi on 4/24/2022.
//

#ifndef PONG_PAINT_WITH_SHADERS_LEVEL_H
#define PONG_PAINT_WITH_SHADERS_LEVEL_H

#include "../Pong/core/core_vals.h"
#include "../Pong/core/level.h"
#include "../Pong/core/material.h"
#include "../Pong/core/lights.h"
#include "../utils/logger.h"
#include "../Pong/material_lib.h"
#include "blending_level.h"
#include "Pong/shapes/plane_shape.h"
#include <vector>

// TODO: quad level for Shader testing
class PaintWithShades : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        AbstractLevel::_level_setup();

        // add a camera quad
        auto draw_shader = _scene->create_shader(
                "draw_shd",
                "./src/shaders/raymarching_shaders/primitives_scene.glsl");

        auto draw_material =
                _scene->create_material<Pong::Material>(
                "draw_mat", draw_shader,
                {});

        // full screen quad
        auto draw_shape = _scene->create_shape<Pong::PlaneShape>(
                "draw_shape", 2.0, 2.0);

        auto draw_actor = _scene->create_actor<Pong::APlayer>("Draw Actor");


        _scene->get_camera()->position = glm::vec3(0.0, 5.0, 5.0);

        _scene->assign_material(draw_material, draw_shape);
        _scene->assign_shape(draw_shape, draw_actor);
    }
};

#endif //PONG_PAINT_WITH_SHADERS_LEVEL_H
