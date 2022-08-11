//
// Created by Jordi on 4/30/2022.
//

#ifndef PONG_TEST_LEVEL_H
#define PONG_TEST_LEVEL_H
#include "../Pong/core/level.h"
#include "../Pong/material_lib.h"
#include "../utils/logger.h"
#include "Pong/shapes/cube_shape.h"

class TestLevel1 : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        AbstractLevel::_level_setup();

        // a cube
        auto grass_shd = _scene->create_shader(
                "cube_shd",
                "../src/shaders/basic.glsl");

        auto cube_mat = _scene->create_material<Pong::MaterialController>(
                "cube_mat",
                grass_shd,
                {});

        auto cube_shp = _scene->create_shape<Pong::CubeShape>(
                "cube_shp", 2.0, 2.0, 2.0);
        auto cube_act_01 = _scene->create_actor<Pong::APlayer>(
                "cube_act_01");

        _scene->assign_material(cube_mat,
                                cube_shp);

        _scene->assign_shape(cube_shp,
                             cube_act_01);

        // get camera_ptr
        _scene->get_camera()->position = glm::vec3(0, 0, 5.0);
    }
};

#endif //PONG_TEST_LEVEL_H
