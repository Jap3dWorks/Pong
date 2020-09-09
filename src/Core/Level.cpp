#include "Level.h"
#include "../logger.h"

#include <vector>

namespace Pong
{
    AbstractLevel::AbstractLevel() {
        _render = Pong::Render::getInstance();
        _scene = Pong::Scene::get_instance();
    }

    AbstractLevel::~AbstractLevel()
    {
        delete _scene;
        delete _render;
    }

    void AbstractLevel::run()
    {
        // --config scene--
        LOG_INFO("Setup level")
        _level_setup();
        LOG_INFO("Collect blending actors.")
        // blending actors are drawn by distance to camera_ptr.
        _scene->collect_blending_actors();
        LOG_INFO("Sort materials, shapes and actors")
        _scene->sort_materials();
        _scene->sort_shapes_maps();
        _scene->sort_actor_maps();
        // --inputs--
        // ----------
        _configInputs();

        // --Run components start methods--
        // implement here
        _components_start_level();

        // render loop
        while (!glfwWindowShouldClose(_render->getWindow()))
        {
            // pre-frame logic
            _render->calculate_deltaTime();

            _update_actors();

            _frame_collisions();

            // inputs
            _close_level();
            for (auto const &i: _inputList)
                i(Pong::Render::DeltaTime);

            _frame_calc();

            Pong::Render::bind_framebuffer(_render->get_framebuffer());
//            Pong::Render::bind_framebuffer(0);
            _frame_draw();
//            glfwSwapBuffers(_render->getWindow());
//            glfwPollEvents();
            _render->draw_framebuffer();
        }
        LOG_DEBUG("Exit lvl");
    }

    void AbstractLevel::_close_level()
    {
        // close level inputs
        if (glfwGetKey(_render->getWindow(),
                GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_render->getWindow(), true);
        }
    }

    void AbstractLevel::_configInputs()
    {
        // camera_ptr inputs
        Render* render = Render::getInstance();
        std::map<int, Pong::Movements> camMap =
        {
            {GLFW_KEY_W, Pong::Movements::FORWARD},
            {GLFW_KEY_S, Pong::Movements::BACKWARD},
            {GLFW_KEY_A, Pong::Movements::LEFT},
            {GLFW_KEY_D, Pong::Movements::RIGHT},
            {GLFW_KEY_E, Pong::Movements::UP},
            {GLFW_KEY_Q, Pong::Movements::DOWN}
        };
        _inputList.emplace_back(
                _scene->get_camera(),
            camMap,
            render->getWindow());
    }

    void AbstractLevel::_frame_collisions()
    {
        // TODO: thread pool to manage collisions
        // --compute collisions--
        auto it = _scene->collider_map.begin();
        unsigned int size = _scene->collider_map.size();
        if(size < 2) return;

        for (int i = 0; i < size - 1; ++i)
        {
            for (int j = i + 1; j < size; ++j)
            {
                if (std::next(it, i)->second->collide(std::next(it, j)->second))
                {
                    for (CollisionComponent*& c : std::next(it, i)->second->component_list)
                    {
                        c->at_collision(std::next(it, i)->second, std::next(it, j)->second);
                    }

                    for (CollisionComponent*& c : std::next(it, j)->second->component_list)
                    {
                        c->at_collision(std::next(it, j)->second, std::next(it, i)->second);
                    }
                }
            }
        }
    }

    void AbstractLevel::_frame_calc(){}

    void AbstractLevel::_frame_draw()
    {
        _render->update_ubo_view_matrices(_scene->get_camera());
        for (auto layer: _render->first_pass_renderlayers){
            for (auto material: _scene->renderlayer_material_map[layer]){
                material->use();
                material->update_shader(_render, _scene);
                for (auto shape: _scene->material_shape_map[material])
                {
                    shape->bind_VAO();
                    for (auto actor: _scene->shape_actor_map[shape])
                    {
                        // Draw by actor
                        if (actor->get_visibility())
                        {
                            actor->draw(_render, _scene, material);
                            shape->draw(_render, _scene, material);
                        }
                    }
                }
            }
        }

        // if backface cull is enabled, disable it to draw transparent objects.
        glDisable(GL_CULL_FACE);
        // Draw first far objects
        _scene->sort_blending_actors();
        for (auto it = _scene->blending_actors.rbegin();
                it != _scene->blending_actors.rend(); it++)
        {
            auto act = *it;
            if (!act->get_visibility()) continue;

            auto shp = _scene->blending_actor_shape_material_map[*it].first;
            auto mat = _scene->blending_actor_shape_material_map[*it].second;
            mat->use();
            mat->update_shader(_render, _scene);
            shp->bind_VAO();
            act->draw(_render, _scene, mat);
            shp->draw(_render, _scene, mat);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        if (_render->gl_enables_config & 4){
            glEnable(GL_CULL_FACE);}
    }

    void AbstractLevel::_level_setup(){}

    void AbstractLevel::_update_actors()
    {
        auto it = _scene->actor_map.begin();
        for (int i = 0; i < _scene->actor_map.size(); i++)
        {
            // try to update only kinetic actors
            std::next(it, i)->second->update(Pong::Render::DeltaTime);
        }
    }

    void AbstractLevel::_components_start_level(){
        for (auto &act: _scene->actor_map)
        {
            for (auto &cmp : act.second->get_components())
            {
                cmp->at_init();
            }
        }

        for (auto &act : _scene->collider_map)
        {
            for (auto &cmp : act.second->get_components())
            {
                cmp->at_init();
            }
        }
    }

    // TestLevel
    // ---------
    void TestLevel::_level_setup() {
        // get camera_ptr
        ACamera *camera = _scene->get_camera();
        camera->Position = glm::vec3(0, 0, 9);

        // build and compile the shader program
        Shader *paint_shd = _scene->create_shader("paint_shd",
                                                  "../shaders/paint_V.glsl",
                                                  "../shaders/paint_F.glsl");

        auto *paint_mat = _scene->create_material<Material>(
                "paint_mat",
                paint_shd,
                {_scene->create_texture("paint_tex",
                                        "../Textures/waterColor.jpg",
                                        "texture1")});

        auto *cube_01 = _scene->create_actor<APlayer>("icosphere_01");
        auto *cube_shp = _scene->create_shape<IcosphereShape>(
                "icosphere_shp",
                1,
                2);
        _scene->assign_material(paint_mat, cube_shp);
        _scene->assign_shape(cube_shp, cube_01);

        Shader *skybox_shd = _scene->create_shader("skybox_shd",
                                                   "../shaders/reflect_skybox_v.glsl",
                                                   "../shaders/reflect_skybox_f.glsl");

        Material *skybox_mat = _scene->create_material<SKyBoxMaterial>(
                "skybox_mat",
                skybox_shd,
                {_scene->create_texture("skybox_tex",
                                        "skybox",
                                        "../Textures/skybox_right.jpg", "../Textures/skybox_left.jpg",
                                        "../Textures/skybox_top.jpg", "../Textures/skybox_bottom.jpg",
                                        "../Textures/skybox_front.jpg", "../Textures/skybox_back.jpg")},
                                        RenderLayer::SKY_BOX);

        // Sky box should be drawn last.
        skybox_mat->order = 900;
        // hack z_skybox to sort at the end of the map.
        auto* skybox_shp = _scene->create_shape<SkyBoxShape>("skybox_shp");
        auto* skybox_act = _scene->create_actor<ASkyBox>("skybox_act");
        _scene->assign_material(skybox_mat,
                                _scene->create_shape<SkyBoxShape>("skybox_shp"));
        _scene->assign_shape(skybox_shp, skybox_act);

        skybox_act->set_visibility(true);

        // --lighting--
        DirectionalLight *directional_light = _scene->get_directional_light();
        directional_light->ambient = glm::vec3{0.1f, 0.1f, 0.05f};
        directional_light->color = glm::vec3{0.8f, 0.8f, 0.3f};
        directional_light->direction = glm::normalize(glm::vec3{0.3f, -1.f, -0.5f});
    }
}