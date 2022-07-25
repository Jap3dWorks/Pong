#include "Pong/core/level.h"
#include "utils/logger.h"
#include "collision_components.h"

#include <vector>

namespace Pong {
    AbstractLevel::AbstractLevel() {
        _render = Pong::Render::get_instance();
        _scene = Pong::Scene::get_instance();
    }

    AbstractLevel::~AbstractLevel() {
        delete _scene;
        delete _render;
    }

    void AbstractLevel::run() {
        LOG_INFO("Clear frame counter");
        _render->clear_frame_count();

        // --config Scene--
        LOG_INFO("Setup level");
        _level_setup();
        LOG_INFO("Collect blending actors.");
        // blending actors are drawn by distance to camera_ptr.
        LOG_INFO("Sort materials, shapes and actors");

        // --inputs--
        _configInputs();

        // --Run components start methods--
        // implement here
        _components_start_level();

        // Render loop
        while (!glfwWindowShouldClose(_render->get_window())) {
            // pre-frame logic
            _update_actors();
            _frame_collisions();

            // inputs
            _close_level();
            for (const auto &item: _inputList)
                item(_render->get_delta_time());

            _frame_calc();

            Pong::Render::bind_framebuffer(_render->get_framebuffer());
            _frame_draw();

            _render->draw_framebuffer();
        }
        LOG_DEBUG("Exit lvl");
    }

    void AbstractLevel::_close_level() {
        // close level inputs
        if (glfwGetKey(_render->get_window(),
                GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_render->get_window(), true);
        }
    }

    void AbstractLevel::_configInputs() {
        // camera_ptr inputs
        Render* render = Render::get_instance();
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
                render->get_window());
    }

    void AbstractLevel::_frame_collisions() {
        // TODO: thread pool to manage collisions
        // --compute collisions--
        auto it = _scene->collider_map.begin();
        unsigned int size = _scene->collider_map.size();
        if(size < 2) return;

        for (int i = 0; i < size - 1; ++i) {
            for (int j = i + 1; j < size; ++j) {
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

    void AbstractLevel::_frame_calc() {}

    void AbstractLevel::_frame_draw() {
        _render->update_ubo_camera(
                _scene->get_camera()
                );
        _render->update_ubo_lights(
                _scene->get_directional_lights(),
                _scene->get_point_lights()
                );
        _render->update_ubo_runtime_data();

        for (auto layer: _render->first_pass_render_layers) {
            for (auto &ord_mat: _scene->renderlayer_material_map[layer]){
                ord_mat.second->use();
                ord_mat.second->update_shader(_render, _scene);
                for (auto &ord_shape: _scene->material_shape_map[ord_mat.second])
                {
                    ord_shape.second->bind();
                    for (auto &ord_act: _scene->shape_actor_map[ord_shape.second])
                    {
                        // Draw by Actor
                        if (ord_act.second->active)
                        {
                            ord_act.second->draw(_render, _scene, ord_mat.second);
                            ord_shape.second->update(_render, _scene, ord_mat.second);
                        }
                    }
                }
            }
        }

        glDisable(GL_CULL_FACE);
        // Draw first far objects
//        _scene->sort_blending_actors();
//        _scene->blending_actors;
        for (const auto &actor_ptr: _scene->camera_sorted_blending_actors()) {
            if (!actor_ptr->active) continue;

            auto shp_mat = _scene->blending_actor_shape_material_map[actor_ptr];
            shp_mat.second->use();
            shp_mat.second->update_shader(_render, _scene);
            shp_mat.first->bind();
            actor_ptr->draw(_render, _scene, shp_mat.second);
            shp_mat.first->update(_render, _scene, shp_mat.second);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        if (_render->gl_enables_config & 4) {
            glEnable(GL_CULL_FACE);
        }

    }

    void AbstractLevel::_level_setup() {}

    void AbstractLevel::_update_actors() {
        auto it = _scene->actor_map.begin();
        for (int i = 0; i < _scene->actor_map.size(); i++)
        {
            // try to update only kinetic actors
            std::next(it, i)->second->update();
        }
    }

    void AbstractLevel::_components_start_level() {
        for (auto &act: _scene->actor_map) {
            for (auto & cmp: act.second->components) {
                cmp->start(act.second);
            }
        }
    }
}
