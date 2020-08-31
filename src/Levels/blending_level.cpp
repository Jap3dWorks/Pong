//
// Created by Jordi on 8/23/2020.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include "../Graphic/Material.h"
#include "../Graphic/Lights.h"


#include "blending_level.h"

void BlendingLevel::_level_setup() {
    AbstractLevel::_level_setup();
    // get camera
    Pong::ACamera *a_camera = _scene->get_camera();
    a_camera->Position = glm::vec3(0, 0, 9);

    // grass planes
    auto grass_shd = _scene->create_shader("grass_shd",
                                           "./Shaders/cutout_V.glsl",
                                           "./Shaders/cutout_F.glsl");

    auto grass_mat = _scene->create_material<Pong::Material>(
            "grass_mat",
            grass_shd,
            {_scene->create_texture("grass_tx",
                                    "./Textures/grass.png",
                                    "texture1")});

    auto grass_shp = _scene->create_shape<Pong::CubeShape>("grass_shp");
    auto grass_act = _scene->create_actor<Pong::APlayer>("grass_act");

    grass_act->set_transform(glm::rotate(grass_act->get_transform(),
                                         (float)M_PI,
                                         glm::vec3(0,0,1)));

    _scene->assign_material(grass_mat, grass_shp);
    _scene->assign_shape(grass_shp, grass_act);

    glBindTexture(GL_TEXTURE_2D, _scene->get_texture("grass_tx")->get_id());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // blending
    _create_blending_actors();

    // skybox
    auto skybox_shd = _scene->create_shader("skybox_shd",
                                            "./Shaders/reflect_skybox_v.glsl",
                                            "./Shaders/reflect_skybox_f.glsl");

    auto *skybox_mat = _scene->create_material<Pong::SKyBoxMaterial>(
            "skybox_mat",
            skybox_shd,
            {_scene->create_texture("skybox_tex",
                                    "skybox",
                                    "./Textures/skybox_right.jpg", "./Textures/skybox_left.jpg",
                                    "./Textures/skybox_top.jpg", "./Textures/skybox_bottom.jpg",
                                    "./Textures/skybox_front.jpg", "./Textures/skybox_back.jpg")},
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

    // config openGL global blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BlendingLevel::_create_blending_actors() {
    // semitransparent
    auto blnd_shader = _scene->create_shader("blnd_shader",
                                             "./Shaders/blending_v.glsl",
                                             "./Shaders/blending_f.glsl");

    auto bld_mat = _scene->create_material<Pong::Material>(
            "bld_mat",
            blnd_shader,
            {_scene->create_texture("bld_tex",
                                    "texture1",
                                    "./Textures/blending_transparent_window.png")}
    );
    auto bld_shp = _scene->create_shape<Pong::CubeShape>(
            "blnd_shp",
            1.f, 1.f, 1.f);

    _scene->assign_layer(Pong::RenderLayer::BLENDING, bld_mat);
    _scene->assign_material(bld_mat, bld_shp);

    glm::vec3 positions[8]{
        glm::vec3(11, 6, -5),
        glm::vec3(-3, 2, -8),
        glm::vec3(6.5, -6, 14),
        glm::vec3(9, -2.1, 5),
        glm::vec3(-7, -5, -9),
        glm::vec3(5, -6, -0.8),
        glm::vec3(1, -9, 20),
        glm::vec3(22, 8, 14),
    };

    for(unsigned int i = 0; i < 8; i ++)
    {
        auto bld_act = _scene->create_actor<Pong::Actor>(
                "bld_act" + std::to_string(i));

        bld_act->set_transform(
                glm::translate(bld_act->get_transform(),
                positions[i]));

        _scene->assign_shape(bld_shp, bld_act);
    }

}