//
// Created by Jordi on 4/30/2022.
//

#ifndef PONG_TEST_LEVEL_H
#define PONG_TEST_LEVEL_H
#include "Core/Level.h"
#include "DefaultMaterials.h"
#include "logger.h"

class TestLevel : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        // get camera_ptr
        Pong::ACamera *camera = _scene->get_camera();
        camera->Position = glm::vec3(0, 0, 9);

        // build and compile the shader program
        Shader *paint_shd = _scene->create_shader("paint_shd",
                                                  "../shaders/paint.glsl");

        auto *paint_mat = _scene->create_material<Pong::Material>(
                "paint_mat",
                paint_shd,
                {_scene->create_texture("paint_tex",
                                        "../Textures/waterColor.jpg",
                                        "texture1")});

        auto *cube_01 = _scene->create_actor<Pong::APlayer>("icosphere_01");
        auto *cube_shp = _scene->create_shape<Pong::IcosphereShape>(
                "icosphere_shp",
                1,
                2);
        _scene->assign_material(paint_mat, cube_shp);
        _scene->assign_shape(cube_shp, cube_01);

        Shader *skybox_shd = _scene->create_shader("skybox_shd",
                                                   "../shaders/reflect_skybox.glsl");

        Pong::Material *skybox_mat = _scene->create_material<Pong::SKyBoxMaterial>(
                "skybox_mat",
                skybox_shd,
                {_scene->create_texture("skybox_tex",
                                        "skybox",
                                        "../Textures/skybox_right.jpg", "../Textures/skybox_left.jpg",
                                        "../Textures/skybox_top.jpg", "../Textures/skybox_bottom.jpg",
                                        "../Textures/skybox_front.jpg", "../Textures/skybox_back.jpg")},
                Pong::RenderLayer::SKY_BOX);

        // Sky box should be drawn last.
        skybox_mat->order = 900;
        // hack z_skybox to sort at the end of the map.
        auto* skybox_shp = _scene->create_shape<Pong::SkyBoxShape>("skybox_shp");
        auto* skybox_act = _scene->create_actor<Pong::ASkyBox>("skybox_act");
        _scene->assign_material(skybox_mat,
                                _scene->create_shape<Pong::SkyBoxShape>("skybox_shp"));
        _scene->assign_shape(skybox_shp, skybox_act);

        skybox_act->set_visibility(true);

        // --lighting--
        Pong::DirectionalLight *directional_light = _scene->get_directional_light();
        directional_light->ambient = glm::vec4{0.1f, 0.1f, 0.05f, 1.f};
        directional_light->color = glm::vec4{0.8f, 0.8f, 0.3f, 1.f};
        directional_light->direction = glm::normalize(glm::vec4{0.3f, -1.f, -0.5f, 1.f});
    }

//public:
//    TestLevel() = default;
};

#endif //PONG_TEST_LEVEL_H
