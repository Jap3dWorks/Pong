//
// Created by Jordi on 8/23/2020.
//

#include "belending_level.h"

void BlendingLevel::_level_setup() {
    AbstractLevel::_level_setup();

    // get camera
    Pong::ACamera *a_camera = _scene->get_camera();
    a_camera->Position = glm::vec3(0, 0, 9);

    // grass planes
    auto grass_shd = _scene->create_shader("grass_shd",
                                           "../../shaders/cutout_v.glsl",
                                           "../../shaders/cutout_f.glsl");

    auto grass_mat = _scene->create_material<Pong::Material>("grass_mat",
                                                             grass_shd,
                                                             {});

    auto skybox_shd = _scene->create_shader("skybox_shd",
                                               "../../shaders/reflect_skybox_v.glsl",
                                               "../../shaders/reflect_skybox_f.glsl");

    auto *skybox_mat = _scene->create_material<Pong::SKyBoxMaterial>(
            "skybox_mat",
            skybox_shd,
            {_scene->create_texture("skybox_tex",
                                    "skybox",
                                    "../textures/skybox_right.jpg", "../textures/skybox_left.jpg",
                                    "../textures/skybox_top.jpg", "../textures/skybox_bottom.jpg",
                                    "../textures/skybox_front.jpg", "../textures/skybox_back.jpg")},
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
    directional_light->ambient = glm::vec3{0.1f, 0.1f, 0.05f};
    directional_light->color = glm::vec3{0.8f, 0.8f, 0.3f};
    directional_light->direction = glm::normalize(
            glm::vec3{0.3f, -1.f, -0.5f});
}
