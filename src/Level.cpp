#include "Level.h"
#include "logger.h"

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
        _level_setup();

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

            _frame_draw();

            // clean buffers
            glfwSwapBuffers(_render->getWindow());
            glfwPollEvents();
        }
        LOG_DEBUG("Exit lvl");
    }

    void AbstractLevel::_close_level()
    {
        // close level inputs
        if (glfwGetKey(_render->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_render->getWindow(), true);
        }
    }

    void AbstractLevel::_configInputs()
    {
        // camera inputs
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
        // --clean render--
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (const auto & mat_pair: _scene->material_shape_map)
        {
            mat_pair.first->update_shader(_render, _scene);
            mat_pair.first->use();
            for (const auto & shape_actor: mat_pair.second)
            {
                glBindVertexArray(shape_actor.first->get_VAO());
                shape_actor.first->draw(_render, _scene, nullptr);  // TODO: bind buffer
                for (const auto & actor: shape_actor.second)
                {
                    actor->draw(_render, _scene, nullptr);
                    shape_actor.first->draw(nullptr, nullptr, nullptr);
                }
            }
            mat_pair.first->end_use();
        }
        glBindVertexArray(0);
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

    void AbstractLevel::_components_start_level()
    {
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
        // get camera
        ACamera *camera = _scene->get_camera();
        camera->Position = glm::vec3(0, 0, 9);

        // build and compile the shader program
        Shader *paint_shd = _scene->create_shader("paint_shd",
                                                  "../shaders/paint_V.glsl",
                                                  "../shaders/paint_F.glsl");

        Material *paint_mat = _scene->create_material(
                "paint_mat",
                paint_shd,
                {_scene->create_texture("paint_tex",
                                        "../textures/waterColor.jpg",
                                        "texture1")});

        auto *cube_01 = _scene->create_actor<APlayer>("cube_01");
        _scene->assign_material(paint_mat,
                                _scene->create_shape<IcosphereShape>("cube_shp", 1, 2),
                                cube_01);

        // Sky box should be drawn last.
        Shader *skybox_shd = _scene->create_shader("skybox_shd",
                                                   "../shaders/reflect_skybox_v.glsl",
                                                   "../shaders/reflect_skybox_f.glsl");

        Material *skybox_mat = _scene->create_material(
                "skybox_mat",
                skybox_shd,
                {_scene->create_texture("skybox_tex",
                                        "skybox",
                                        "../textures/skybox_right.jpg", "../textures/skybox_left.jpg",
                                        "../textures/skybox_top.jpg", "../textures/skybox_bottom.jpg",
                                        "../textures/skybox_front.jpg", "../textures/skybox_back.jpg")});

        // hack z_skybox to sort at the end of the map
        _scene->assign_material(skybox_mat,
                                _scene->create_shape<SkyBoxShape>("skybox_shp"),
                                _scene->create_actor<ASkyBox>("z_skybox_act"));

        // --lighting--
        DirectionalLight *directional_light = _scene->get_directional_light();
        directional_light->ambient = glm::vec3{0.1f, 0.1f, 0.05f};
        directional_light->color = glm::vec3{0.8f, 0.8f, 0.3f};
        directional_light->direction = glm::normalize(glm::vec3{0.3f, -1.f, -0.5f});
    }

    void TestLevel::_barycentric_tst()
    {
        glm::vec3 v_a = glm::normalize(glm::vec3(1, 1, 1));
/*
        glm::mat4 tr_m(1);
        tr_m = glm::rotate(tr_m, 3.14f/2, glm::normalize(glm::vec3(0, 1, 0)));*/

        // glm::mat3 m = glm::mat3(tr_m);

        glm::vec3 v_var(glm::mat3(_scene->get_camera()->get_view_matrix()) * v_a);

        // cout_vector(v_var);
    }

    void TestLevel::_frame_calc()
    {
        return;
        // shot markers
        shot_delay += Pong::Render::DeltaTime;
        if (shot_delay >= .2f
            && (glfwGetKey(_render->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS))
        {
            if (true) {
                _barycentric_tst();
            }
            else
            {
                ACamera* cam = _scene->get_camera();
                //glm::mat4 cam_trns = cam->get_view_matrix();
                RayCast ray(glm::vec3(cam->Front.x, cam->Front.y, cam->Front.z),
                    cam->Position);

                std::vector<RayCastData> rcd;
                for (auto &coll : _scene->collider_map)
                {
                    coll.second->ray_cast(ray, rcd);
                }
                if (rcd.size())
                {
                    sort_raycast_data(rcd, ray.position, 0, rcd.size() - 1);
                    draw_point(rcd[0].point);
                }
            }
            shot_delay = 0.f;
        }
    }
}