#include "Level.h"
#include "logger.h"

#include "Utils.h"

#include <vector>
#include <GLFW/glfw3.h>


namespace Pong
{
    AbstractLevel::AbstractLevel() {
        _render = Render::getInstance();
        _scene = Scene::getInstance();
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
            _scene->getCamera(),
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

        // draw actors
        for (const auto& kv : _scene->actor_map)
        {
            _update_shader(kv.second->getMaterial()->get_shader());
            kv.second->draw();
        }
    }

    void AbstractLevel::_update_shader(Shader* shader)
    {
        shader->setMat4("view", _scene->getCamera()->GetViewMatrix());
        shader->setMat4("projection", glm::perspective(glm::radians(_scene->getCamera()->Zoom),
            (float)Pong::Render::SCR_WIDTH / (float)Pong::Render::SCR_HEIGHT,
            0.1f,
            10000.f));

        // directional light
        shader->setVec3("directional.Direction", _scene->getDirectionalLight()->direction);
        shader->setVec3("directional.Color", _scene->getDirectionalLight()->color);
        shader->setVec3("directional.Ambient", _scene->getDirectionalLight()->ambient);

        // points lights
        for (unsigned int i = 0; i < Pong::Scene::POINT_LIGHTS; ++i)
        {
            shader->setVec3("pointLightPositions[" + std::to_string(i) + "]",
                    _scene->get_point_light(i).position);
            shader->setVec3("pointLightColors[" + std::to_string(i) + "]",
                    _scene->get_point_light(i).color);
        }
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


    // Pong level
    // ----------
    void PongLevel::run()
    {
        // get camera
        Camera* camera = _scene->getCamera();
        camera->Position=glm::vec3(0,0,18);

        // build and compile the shader program
        Shader blinn_shader(
                "../shaders/blinn_v.glsl",
                "../shaders/blinn_f.glsl");

        Pong::Material material("blinn_mat", &blinn_shader, std::vector<Pong::Texture*>());
        material.set_param("glow", 64.f);
        material.set_param("specular", 0.85f);
        material.set_param("surfaceColor", glm::vec3{ 0.3,0.3,0.5 });

        // --config players--
        glm::mat4 iniPos = glm::mat4(1);
        glm::vec3 pScale(0.5f, 2.5f, 0.5f);

        auto* p1 = _scene->createActor<APlayer>("p1_ply");
        p1->setShape(_scene->createShape<CubeShape>("cube_shp"));
        p1->setTransform(glm::translate(iniPos, glm::vec3(10.f, 0, 0)));
        p1->setMaterial(&material);
        p1->setScale(pScale);
        LOG_DEBUG("Set collider p1_coll");
        p1->setCollider(_scene->createCollider<BoxCollider>("p1_coll"));

        auto* p2 = _scene->createActor<APlayer>("p2_ply");
        p2->setShape(_scene->getShape("cube_shp"));
        p2->setTransform(glm::translate(iniPos, glm::vec3(-10.f, 0, 0)));
        p2->setMaterial(&material);
        p2->setScale(pScale);
        LOG_DEBUG("Set collider p2_coll");
        p2->setCollider(_scene->createCollider<BoxCollider>("p2_coll"));

        // --config walls--
        glm::vec3 wall_scale = glm::vec3(20, 5, 5);
        auto* upper_wall = _scene->createActor<Actor>("upper_wall");
        upper_wall->setShape(_scene->createShape<CubeShape>("wall_shp"));
        upper_wall->setTransform(glm::translate(iniPos, glm::vec3(0, 9, 0)));
        upper_wall->setMaterial(&material);
        upper_wall->setScale(wall_scale);
        LOG_DEBUG("Set collider upper_wall_coll")
        upper_wall->setCollider(_scene->createCollider<BoxCollider>("upper_wall_coll"));

        auto* lower_wall = _scene->createActor<Actor>("lower_wall");
        lower_wall->setShape(_scene->getShape("wall_shp"));
        lower_wall->setTransform(glm::translate(iniPos, glm::vec3(0, -9, 0)));
        lower_wall->setMaterial(&material);
        lower_wall->setScale(wall_scale);
        LOG_DEBUG("Set collider lower_wall_coll")
        lower_wall->setCollider(_scene->createCollider<BoxCollider>("lower_wall_coll"));

        // -- config out game area reset --
        glm::vec3 out_scale = glm::vec3(5, 20, 20);
        auto* right_outGame = _scene->createActor<Actor>("right_outGame");
        //right_outGame->setShape(_scene->createShape<CubeShape>("out_shp"));
        right_outGame->setTransform(glm::translate(iniPos, glm::vec3(15, 0, 0)));
        right_outGame->setMaterial(&material);
        right_outGame->setScale(out_scale);
        auto* r_out_coll = _scene->createCollider<BoxCollider>("right_out_coll");
        right_outGame->setCollider(r_out_coll);

        BorderCollisionComponent r_border_component;
        r_out_coll->add_component(&r_border_component);

        auto* left_outGame = _scene->createActor<Actor>("left_outGame");
        //left_outGame->setShape(_scene->getShape("out_shp"));
        left_outGame->setTransform(glm::translate(iniPos, glm::vec3(-15, 0, 0)));
        left_outGame->setMaterial(&material);
        left_outGame->setScale(out_scale);
        auto* l_out_coll = _scene->createCollider<BoxCollider>("left_out_coll");
        left_outGame->setCollider(l_out_coll);

        BorderCollisionComponent l_border_component;
        l_out_coll->add_component(&l_border_component);

        // --config ball--
        float radius = .75f;
        auto ball = _scene->createActor<ABall>("ball");
        ball->set_direction(glm::vec3(1, 1, 0));

        // smooth ball doen't work
        auto ball_shp = _scene->createShape<IcosphereShape>(
                "ball_shape",
                radius,
                2,
                true);

        ball->setShape(ball_shp);

        auto* ball_col = _scene->createCollider<SphereCollider>("ball_col");
        ball_col->setRadius(radius);
        ball->setCollider(ball_col);
        ball->setMaterial(&material);
        ball->setVelocity(1.f);

        BallCollisionComponent ball_component;
        ball_col->add_component(&ball_component);

        // --test rt--
        float mark_r = .5f;
        auto* mark = _scene->createActor<AKinetic>("mark");
        auto* mark_shp = _scene->createShape<IcosphereShape>("mark_shp");
        mark_shp->set_radius(mark_r);
        mark->setShape(mark_shp);
        mark->setMaterial(&material);

        // --config lighting--
        Pong::DirectionalLight* directional_light = _scene->getDirectionalLight();
        directional_light->ambient = glm::vec3{ 0.1f, 0.1f, 0.05f };
        directional_light->color = glm::vec3{ 0.8f, 0.8f, 0.3f };
        directional_light->direction = glm::normalize(glm::vec3{ 0.3f, -1.f, -0.5f });

        // --inputs--
        // ----------
        _configInputs();

        // --Run components start methds--
        // implement here

        // ------
        LOG_DEBUG(p1->getMaterial()->get_shader()->ID << " shader id");

        glm::vec3 dir = glm::vec3(-1.f,0,0);
        float variation = 0.f;

        // render loop
        while (!glfwWindowShouldClose(_render->getWindow()))
        {
            // pre-frame logic
            _render->calculate_deltaTime();

            // inputs
            _close_level();
            for (auto const &i : _inputList)
                i(Pong::Render::DeltaTime);

            p1->update(Pong::Render::DeltaTime);
            p2->update(Pong::Render::DeltaTime);
            ball->update(Pong::Render::DeltaTime);


            // --compute collisions--
            _frame_collisions();

            // --clean render--
            glClearColor(0.1f, 0.1f, 0.1f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // --update camera pos--
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 plane_model = glm::mat4(1);
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom),
                (float)Pong::Render::SCR_WIDTH / (float)Pong::Render::SCR_HEIGHT,
                0.1f,
                10000.f);

            blinn_shader.setMat4("view", view);
            blinn_shader.setMat4("projection", projection);

            // directional light;
            blinn_shader.setVec3("directional.Direction", directional_light->direction);
            blinn_shader.setVec3("directional.Color", directional_light->color);
            blinn_shader.setVec3("directional.Ambient", directional_light->ambient);

            // print actors
            for (const auto& kv : _scene->actor_map)
            {
                kv.second->draw();
            }

            // clean buffers
            glfwSwapBuffers(_render->getWindow());
            glfwPollEvents();
        }
    }

    void PongLevel::_configInputs()
    {
        // calls super class configinputs method
        AbstractLevel::_configInputs();

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
            _scene->getCamera(),
            camMap,
            render->getWindow());

        // p1 inputs
        std::map<int, Pong::Movements> p1Map =
        {
            {GLFW_KEY_O, Pong::Movements::UP },
            {GLFW_KEY_L, Pong::Movements::DOWN}
        };
        _inputList.emplace_back(
            _scene->getActor("p1_ply"),
            p1Map,
            render->getWindow());

        // p2 inputs
        std::map<int, Pong::Movements> p2Map =
        {
            {GLFW_KEY_U, Pong::Movements::UP},
            {GLFW_KEY_J, Pong::Movements::DOWN}
        };
        _inputList.emplace_back(
            _scene->getActor("p2_ply"),
            p2Map,
            render->getWindow());
    }


    // TestLevel
    // ---------
    void TestLevel::_level_setup()
    {
        // get camera
        Camera* camera = _scene->getCamera();
        camera->Position = glm::vec3(0, 0, 9);

        // build and compile the shader program
        Shader* blinn_shd = _scene->create_shader("blinn_shd",
            "../shaders/blinn_V.glsl",
            "../shaders/blinn_F.glsl");
        //Shader* blinn_shd = _scene->create_shader("blinn_shd", "unlit_v.glsl", "unlit_f.glsl");

        Material* blinn_mat = _scene->createMaterial("blinn_mat", blinn_shd,
            std::vector<Pong::Texture*>());

        blinn_mat->set_param("glow", 64.f);
        blinn_mat->set_param("specular", 0.85f);
        blinn_mat->set_param("surfaceColor", glm::vec3{ 0.3,0.3,0.5 });

        //Shader* paint_shd = _scene->create_shader("paint_shd", "paint_V.glsl", "paint_F.glsl");
        Shader* paint_shd = _scene->create_shader("paint_shd",
            "../shaders/paint_V.glsl",
            "../shaders/paint_F.glsl");

        std::vector<Texture*> paint_tex = { _scene->create_texture(
            "waterColor", "../textures/waterColor.jpg", "texture1") };
        Material* paint_mat = _scene->createMaterial("paint_mat", paint_shd, paint_tex);

        // --config scene--
        glm::mat4 iniPos = glm::mat4(1);
        glm::vec3 pScale(1.f, 1.f, 1.f);

        APlayer* cube_01 = _scene->createActor<APlayer>("cube_01");
        cube_01->setShape(_scene->createShape<CubeShape>("cube_shp"));
        cube_01->setTransform(glm::translate(iniPos, glm::vec3(5,0,0)));
        cube_01->setMaterial(blinn_mat);
        cube_01->setScale(pScale);
        cube_01->setCollider(_scene->createCollider<BoxCollider>("cube_01_coll"));

        APlayer* cube_02 = _scene->createActor<APlayer>("cube_02");
        cube_02->setShape(_scene->getShape("cube_shp"));
        cube_02->setTransform(glm::translate(iniPos, glm::vec3(-5.f, 0, 0)));
        cube_02->setMaterial(blinn_mat);
        cube_02->setScale(pScale);
        cube_02->setCollider(_scene->createCollider<BoxCollider>("cube_02_coll"));

        // --test rt--
        float mark_r = .5f;
        AKinetic* mark = _scene->createActor<AKinetic>("mark");
        IcosphereShape* mark_shp = _scene->createShape<IcosphereShape>("mark_shp");
        mark_shp->set_radius(mark_r);
        mark->setShape(mark_shp);
        mark->setMaterial(paint_mat);
        SphereCollider* m_coll = _scene->createCollider<SphereCollider>("mark_coll");
        mark->setCollider(m_coll);
        m_coll->setRadius(mark_r);

        // --lighting--
        Pong::DirectionalLight* directional_light = _scene->getDirectionalLight();
        directional_light->ambient = glm::vec3{ 0.1f, 0.1f, 0.05f };
        directional_light->color = glm::vec3{ 0.8f, 0.8f, 0.3f };
        directional_light->direction = glm::normalize(glm::vec3{ 0.3f, -1.f, -0.5f });

        // --vector baricentric test --
    }

    void TestLevel::_barycentric_tst()
    {
        glm::vec3 v_a = glm::normalize(glm::vec3(1, 1, 1));
/*
        glm::mat4 tr_m(1);
        tr_m = glm::rotate(tr_m, 3.14f/2, glm::normalize(glm::vec3(0, 1, 0)));*/

        //glm::mat3 m = glm::mat3(tr_m);

        glm::vec3 v_var(glm::mat3(_scene->getCamera()->GetViewMatrix()) * v_a);

//        cout_vector(v_var);
    }

    void TestLevel::_frame_calc()
    {
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
                Camera* cam = _scene->getCamera();
                //glm::mat4 cam_trns = cam->GetViewMatrix();
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


    // PBRLevel
    // --------
    void PBRLevel::_level_setup()
    {
        // get camera
        Camera* camera = _scene->getCamera();
        camera->Position = glm::vec3(0, 0, 9);

        // build and compile the shader program
        Shader* pbr_shd = _scene->create_shader("pbr_shd",
            "../shaders/pbr_basic_V.glsl",
            "../shaders/pbr_basic_F.glsl");

        std::vector<Texture*> pbr_textures = {
            _scene->create_texture("riron2_baseColor",
                    "../textures/rustediron2_basecolor.png",
                    "albedoMap"),
            _scene->create_texture("riron2_metallic",
                    "../textures/rustediron2_metallic.png",
                    "metallicMap"),
            _scene->create_texture("riron2_normal",
                    "../textures/rustediron2_normal.png",
                    "normalMap"),
            _scene->create_texture("riron2_roughness",
                    "../textures/rustediron2_roughness.png",
                    "roughnessMap"),
            _scene->create_texture("ao_map",
                    "../textures/aoMap.png",
                    "aoMap")
        };

        Material* pbr_mat = _scene->createMaterial("pbr_mat", pbr_shd, pbr_textures);

        // --config scene--
        glm::mat4 iniPos = glm::mat4(1);
        glm::vec3 pScale(1.f, 1.f, 1.f);

        auto cube_01 = _scene->createActor<APlayer>("cube_01");
        cube_01->setShape(_scene->createShape<CubeShape>("cube_shp"));
        cube_01->setTransform(glm::translate(iniPos, glm::vec3(5, 0, 0)));
        cube_01->setMaterial(pbr_mat);
        cube_01->setScale(pScale);
        LOG_DEBUG("Set collider");
        cube_01->setCollider(_scene->createCollider<BoxCollider>("cube_01_coll"));

        auto cube_02 = _scene->createActor<APlayer>("cube_02");
        cube_02->setShape(_scene->getShape("cube_shp"));
        cube_02->setTransform(glm::translate(iniPos, glm::vec3(-5.f, 0, 0)));
        cube_02->setMaterial(pbr_mat);
        cube_02->setScale(pScale);
        cube_02->setCollider(_scene->createCollider<BoxCollider>("cube_02_coll"));

        // --test rt--
        float mark_r = .5f;
        auto mark = _scene->createActor<AKinetic>("mark");
        auto mark_shp = _scene->createShape<IcosphereShape>("mark_shp");
        mark_shp->set_radius(mark_r);
        mark->setShape(mark_shp);
        mark->setMaterial(pbr_mat);
        auto m_coll = _scene->createCollider<SphereCollider>("mark_coll");
        mark->setCollider(m_coll);
        m_coll->setRadius(mark_r);

        // --lighting--
         Pong::DirectionalLight* directional_light = _scene->getDirectionalLight();
         directional_light->ambient = glm::vec3{ 0.1f, 0.1f, 0.05f };
         directional_light->color = glm::vec3{ 0.8f, 0.8f, 0.3f };
         directional_light->direction = glm::normalize(glm::vec3{ 0.3f, -1.f, -0.5f });

        // create point lights
        _scene->get_point_light(0).color = glm::vec3(10.f, 10.f, 10.f);
        _scene->get_point_light(0).position = glm::vec3(0.f, 0.f, 10.f);
    }


    // Model level
    // -----------
    void ModelLevel::_level_setup()
    {
        // get camera
        Camera* camera = _scene->getCamera();
        camera->Position = glm::vec3(0, 0, 9);

        // build and compile the shader program
        Shader* blinn_shd = _scene->create_shader("blinn_shd",
                                                  "../shaders/blinn_V.glsl",
                                                  "../shaders/blinn_F.glsl");

        Material* blinn_mat = _scene->createMaterial("blinn_mat", blinn_shd,
                                                     std::vector<Pong::Texture*>());

        blinn_mat->set_param("glow", 64.f);
        blinn_mat->set_param("specular", 0.85f);
        blinn_mat->set_param("surfaceColor", glm::vec3{ 0.3,0.3,0.5 });

        // --config scene--
        glm::mat4 iniPos = glm::mat4(1);
        glm::vec3 pScale(1.f, 1.f, 1.f);

        auto sphere = _scene->createActor<Actor>("cube_02");
        sphere->setShape(_scene->createShape<IcosphereShape>("cube_shp",
                                                             2.f,
                                                             1,
                                                             true));

        sphere->setTransform(glm::translate(iniPos, glm::vec3(0.f, 0, 0)));
        sphere->setMaterial(blinn_mat);
        sphere->setScale(pScale);

        // TODO if sphere doesn't has collider scene doesn't work
//        sphere->setCollider(_scene->createCollider<SphereCollider>("cube_02_coll"));

        // --lighting--
        Pong::DirectionalLight* directional_light = _scene->getDirectionalLight();
        directional_light->ambient = glm::vec3{ 0.1f, 0.1f, 0.05f };
        directional_light->color = glm::vec3{ 0.8f, 0.8f, 0.3f };
        directional_light->direction = glm::normalize(glm::vec3{ 0.3f, -1.f, -0.5f });
    }
}