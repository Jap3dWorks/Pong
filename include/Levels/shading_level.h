//
// Created by Jordi on 4/24/2022.
//

#ifndef PONG_SHADING_LEVEL_H
#define PONG_SHADING_LEVEL_H

#include "Core/core_vals.h"
#include "Core/Level.h"
#include "Core/core_vals.h"
#include "Core/Material.h"
#include "Core/Lights.h"
#include "logger.h"
#include "DefaultMaterials.h"
#include "Levels/blending_level.h"
#include <vector>

// TODO: quad level for shader testing
class ShadingLevel : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        AbstractLevel::_level_setup();

        // add a camera quad
        auto draw_shader = _scene->create_shader(
                "draw_shd",
                "../Shaders/landscape.glsl");

        auto draw_material =
                _scene->create_material<Pong::Material>(
                "draw_mat", draw_shader,
                {});

        auto draw_shape = _scene->create_shape<Pong::PlaneShape>("draw_shape");
        auto draw_actor = _scene->create_actor<Pong::APlayer>("Draw Actor");

        _scene->assign_material(draw_material, draw_shape);
        _scene->assign_shape(draw_shape, draw_actor);
//        draw_actor->set_scale(
//                {2.0, 2.0, 2.0}
//                );
    }
};

#endif //PONG_SHADING_LEVEL_H
