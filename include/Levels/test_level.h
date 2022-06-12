//
// Created by Jordi on 4/30/2022.
//

#ifndef PONG_TEST_LEVEL_H
#define PONG_TEST_LEVEL_H
#include "Pong/core/Level.h"
#include "Pong/default_materials.h"
#include "Pong/logger.h"
#include "Pong/shapes/cube_shape.h"

class TestLevel1 : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        AbstractLevel::_level_setup();

        // get camera_ptr
        auto a_camera = _scene->get_camera();
        a_camera->Position = glm::vec3(0, 0, 10);

        // a cube
        auto grass_shd = _scene->create_shader(
                "cube_shd",
                "./Shaders/basic.glsl");

        auto cube_mat = _scene->create_material<Pong::Material>(
                "cube_mat",
                grass_shd,
                {});

        auto cube_shp = _scene->create_shape<Pong::PlaneShape>("cube_shp");
        auto cube_act_01 = _scene->create_actor<Pong::APlayer>("cube_act_01");

        cube_act_01->set_transform(
                glm::mat4(1.f, 0.f,  0.f, 0.f,
                          0.f, 1.f,  0.f, 0.f,
                          0.f, 0.f,  1.f, 0.f,
                          0.f, 0.f, -5.f, 1.f)
        );

        _scene->assign_material(cube_mat,
                                cube_shp);

        _scene->assign_shape(cube_shp,
                             cube_act_01);
    }
};

#endif //PONG_TEST_LEVEL_H
