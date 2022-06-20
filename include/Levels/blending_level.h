//
// Created by Jordi on 8/23/2020.
//
#ifndef PONG_BLENDING_LEVEL_H
#define PONG_BLENDING_LEVEL_H

#include "Pong/core/core_vals.h"
#include "Pong/core/level.h"
#include "Pong/core/core_vals.h"
#include "Pong/core/material.h"
#include "Pong/core/texture.h"
#include "Pong/core/lights.h"
#include "Pong/logger.h"
#include "Pong/shapes/shape_primitives.h"
#include "Pong/default_materials.h"
#include "Levels/blending_level.h"
#include "Pong/shapes/plane_shape.h"

class BlendingLevel : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        AbstractLevel::_level_setup();

        // Enable GL_PROGRAM_POINT_SIZE
        _render->gl_enables_config |= 8;
        _render->update_enables();
        // get camera_ptr
        Pong::ACamera *a_camera = _scene->get_camera();
        a_camera->Position = glm::vec3(0, 0, 9);

        // grass planes
        auto grass_shd = _scene->create_shader("grass_shd",
                                               "./Shaders/cutout.glsl");

        auto grass_mat = _scene->create_material<Pong::Material>(
                "grass_mat",
                grass_shd,
                {
                    {
                        "texture1",
                        _scene->create_texture("grass_tx",
                                               "./Textures/grass.png")
                    }
                });

        auto grass_shp = _scene->create_shape<
                Pong::PlaneShape>("grass_shp");
        auto grass_act_01 = _scene->create_actor<
                Pong::APlayer>("grass_act_01");
        auto grass_act_02 = _scene->create_actor<
                Pong::APlayer>("grass_act_02");

        grass_act_01->set_transform(glm::rotate(grass_act_01->get_transform(),
                                                (float)_P_PI,
                                                glm::vec3(0,0,1)));

        grass_act_02->set_transform(glm::translate(glm::mat4(1.f),
                                                   glm::vec3(2, 0, 1)));

        _scene->assign_material(grass_mat, grass_shp);
        _scene->assign_shape(grass_shp, grass_act_01);
        _scene->assign_shape(grass_shp, grass_act_02);

        _scene->get_texture("grass_tx")->set_wrap(
                TextureWrap::CLAMP_TO_EDGE,
                TextureWrap::CLAMP_TO_EDGE
        );

        // blending
        _create_blending_actors();

//        // vertex size
//        auto vert_shd = _scene->create_shader("vert_shd",
//                                              "./Shaders/point.glsl");
//        auto vert_mat = _scene->create_material<Pong::Material>("vert_mat",
//                                                                vert_shd,
//                                                                {});
//        auto vert_shp = _scene->create_shape<Pong::IcosphereShape>("vert_shp", 2.0);
//        vert_shp->draw_primitive = GL_POINTS;
//        LOG_DEBUG("Change vert_shp to GL_POINTS primitive " << GL_POINTS);
//        auto vert2_shp = _scene->create_shape<Pong::IcosphereShape>("vert2_shp", 2.0);
//        vert2_shp->draw_primitive = GL_LINE_STRIP;
//        LOG_DEBUG("Change vert2_shp to GL_LINE_STRIP primitive " << GL_LINE_STRIP);
//
//        LOG_DEBUG("GL_TRIANGLES id " << GL_TRIANGLES);
//
//        auto vert_act = _scene->create_actor<Pong::Actor>("vert_act");
//        _scene->assign_material(vert_mat, vert_shp);
//        _scene->assign_material(vert_mat, vert2_shp);
//        _scene->assign_shape(vert_shp, vert_act);
//        _scene->assign_shape(vert2_shp, vert_act);
//        vert_act->set_transform(glm::translate(vert_act->get_transform(), glm::vec3(4, 0, 0)));

        // skybox
        auto skybox_shd = _scene->create_shader("skybox_shd",
                                                "./Shaders/reflect_skybox.glsl");

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

        // a blinn material
        auto blinn_shd = _scene->create_shader("blinn_shd",
                                               "./Shaders/blinn.glsl");

        auto blinn_mat = _scene->create_material<Pong::Material>("blinn_mat",
                                                                 blinn_shd,
                                                                 {});

        blinn_mat->set_param("surfaceColor", glm::vec3(0.1, 0.2, 0.3));
        blinn_mat->set_param("glow", 16.f);
        blinn_mat->set_param("specular", .8f);

//        auto blinn_shp = _scene->create_shape<Pong::IcosphereShape>("blinn_shp");
//        _scene->assign_material(blinn_mat, blinn_shp);
//        _scene->assign_shape(blinn_shp, vert_act);

        // TODO: https://learnopengl.com/Advanced-OpenGL/Instancing

        // --lighting--
        Pong::DirectionalLight *directional_light = _scene->get_directional_light();
        directional_light->direction =
                glm::normalize(glm::vec4{0.3f, -1.f, -0.5f, 1.f});
        directional_light->color =
                glm::vec4{0.8f, 0.8f, 0.3f, 1.f};
        directional_light->ambient =
                glm::vec4{0.22f, 0.22f, 0.7f, 1.f};
    }

    inline void _create_blending_actors() {
        // semitransparent
        auto blnd_shader = _scene->create_shader("blnd_shader",
                                                 "./Shaders/blending.glsl");

        auto bld_mat = _scene->create_material<Pong::Material>(
                "bld_mat",
                blnd_shader,
                {{"texture1",
                  _scene->create_texture("bld_tex", "./Textures/blending_transparent_window.png",
                  )}
                },

                Pong::RenderLayer::BLENDING);

        auto bld_shp = _scene->create_shape<Pong::PlaneShape>(
                "blnd_shp",
                1.f, 1.f);

        _scene->assign_material(bld_mat, bld_shp);

        glm::vec3 positions[8]{
                glm::vec3(11.f, 6.f, 0),  // 0
                glm::vec3(11.f, 6.f, 5),  // 1
                glm::vec3(11.f, 6.f, -10.f),  // 2
                glm::vec3(-7.f, -5.f, -9.f),  // 3
                glm::vec3(-3.f, 2.f, -8.f),  // 4
                glm::vec3(-4.f, -9.f, -20.f), // 5
                glm::vec3(5.f, -6.f, -0.8f),  // 6
                glm::vec3(8.f, 8.f, 14.f),
        };

        for(unsigned int i = 0; i < 8; i ++)
        {
            auto bld_act = _scene->create_actor<Pong::APlayer>(
                    "bld_act" + std::to_string(i));

            bld_act->set_transform(
                    glm::translate(bld_act->get_transform(),
                                   positions[i]));

            _scene->assign_shape(bld_shp, bld_act);
        }

    }

};


#endif //PONG_BLENDING_LEVEL_H
