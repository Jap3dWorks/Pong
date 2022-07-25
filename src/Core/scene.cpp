#include "Pong/core/scene.h"

#include <utility>
#include "utils/logger.h"
#include "Pong/material_lib.h"


namespace Pong{
//    Scene* Scene::instance = nullptr;

    Scene::~Scene() {
        LOG_DEBUG("Call Scene destructor");
        auto ait = actor_map.begin();
        for (int i = 0; i < actor_map.size(); i++)
        {
            delete std::next(ait, i)->second;
        }

        auto cit = collider_map.begin();
        for (int i = 0; i < collider_map.size(); i++)
        {
            delete std::next(cit, i)->second;
        }

        auto sh_it = shader_map.begin();
        for (int i = 0; i < shader_map.size(); i++)
        {
            delete std::next(sh_it, i)->second;
        }
    }

    Scene* Scene::get_instance() {
        if (!instance) {
            instance = std::unique_ptr<Scene>(new Scene);
        }
        return instance.get();
    }

    Actor* Scene::get_actor(const std::string& name) const
    {
        if (actor_map.find(name) != actor_map.end())
        {
            return actor_map.at(name);
        }
        else
        {
            return nullptr;
        }

    }

    // -- Colliders--
    Collider* Scene::get_collider(const std::string& name) const
    {
        if (collider_map.find(name) != collider_map.end())
        {
            return collider_map.at(name);
        }
        else
            return nullptr;
    }

    Shader* Scene::create_shader(const std::string &name,
                                 const GLchar *shader_path) {
        if (shader_map.find(name) == shader_map.end())
        {
            auto* ptr = new Shader(shader_path);
            shader_map[name] = ptr;
            return ptr;
        }
        else
            return shader_map[name];
    }

    Shader* Scene::get_shader(const std::string& name) const {
        if (shader_map.find(name) != shader_map.end())
        {
            return shader_map.at(name);
        }
        else
            return nullptr;
    }

    void Scene::collect_blending_actors() {
        blending_actors.clear();
        blending_actor_shape_material_map.clear();
        for (auto &ord_mat: renderlayer_material_map[RenderLayer::BLENDING])
        {
            for(auto &ord_shp: material_shape_map[ord_mat.second])
            {
                for(auto &ord_act: shape_actor_map[ord_shp.second])
                {
                    blending_actors.push_back(ord_act.second);
                    blending_actor_shape_material_map[ord_act.second] =
                            {ord_shp.second, ord_mat.second};
                }
            }
        }
    }

    // TODO: move to input callback functions file
    // callback functions
    // -----------------
    void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
    {
        Scene* scene = Scene::get_instance();
        if (scene->cam_firstMouse)
        {
            scene->cam_lastX = x_pos;
            scene->cam_lastY = y_pos;
            scene->cam_firstMouse = false;
        }

        float x_offset = x_pos - scene->cam_lastX;
        float y_offset = scene->cam_lastY - y_pos;

        scene->cam_lastX = x_pos;
        scene->cam_lastY = y_pos;

        scene->get_camera()->process_mouse_movement(x_offset, y_offset);
    }

    void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
    {
        Scene::get_instance()->get_camera()->process_mouse_scroll(y_offset);
    }

}