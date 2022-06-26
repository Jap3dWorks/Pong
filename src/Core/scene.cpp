#include "Pong/core/scene.h"

#include <utility>
#include "Pong/logger.h"
#include "Pong/default_materials.h"


namespace Pong{
    Scene* Scene::instance = nullptr;

    Scene::Scene() {
        // start Render
        Render* render = Render::get_instance();
        // create callbacks
        glfwSetCursorPosCallback(render->get_window(), Pong::mouse_callback);
        glfwSetScrollCallback(render->get_window(), scroll_callback);
    }

    Scene::~Scene() {
        LOG_DEBUG("Call Scene destructor");
//        delete _directionalLight;

        auto ait = actor_map.begin();
        for (int i = 0; i < actor_map.size(); i++)
        {
            delete std::next(ait, i)->second;
        }

        auto mit = material_map.begin();
        for (int i = 0; i < material_map.size(); i++)
        {
            delete std::next(mit, i)->second;
        }

        auto cit = collider_map.begin();
        for (int i = 0; i < collider_map.size(); i++)
        {
            delete std::next(cit, i)->second;
        }

//        auto sit = shape_map.begin();
//        for (int i = 0; i < shape_map.size(); i++)
//        {
//            delete std::next(sit, i)->second;
//
//        }

        auto sh_it = shader_map.begin();
        for (int i = 0; i < shader_map.size(); i++)
        {
            delete std::next(sh_it, i)->second;
        }

        auto tx_it = textures_map.begin();
        for (int i = 0; i < textures_map.size(); i++)
        {
            delete std::next(tx_it, i)->second;
        }
    }

    Scene* Scene::get_instance() {
        if (!instance)
        {
            instance = new Scene;
        }
        return instance;
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

    Shader* Scene::get_shader(const std::string& name) const
    {
        if (shader_map.find(name) != shader_map.end())
        {
            return shader_map.at(name);
        }
        else
            return nullptr;
    }


    Material* Scene::get_material(const std::string& name) const
    {
        if (material_map.find(name) != material_map.end())
        {
            return material_map.at(name);
        }
        else
            return nullptr;
    }

    // --Textures--
    Texture* Scene::create_texture(const std::string& name,
                                   const std::string& path)
    {
        if (textures_map.find(name) == textures_map.end())
        {
            auto* t_ptr = new Texture(name, path);
            textures_map[name] = t_ptr;

            return textures_map[name];
        }
        else
        {
            return textures_map[name];
        }
    }

    Texture* Scene::get_texture(const std::string& name) const
    {
        if (textures_map.find(name) != textures_map.end())
        {
            return textures_map.at(name);
        }
        else
            return nullptr;
    }

    // import model method
    void Scene::assign_material(Material * material, GraphicShape * shape) {
        material_shape_map[material].push_back(shape);
    }

    void Scene::assign_shape(GraphicShape * shape, Actor * actor) {
        shape_actor_map[shape].push_back(actor);
    }

    void Scene::sort_shapes_maps() {
//        std::sort(shape_order.begin(),
//                shape_order.end(),
//                OrderComparer<GraphicShape *>());

        for (auto &pair: material_shape_map) {
            std::sort(pair.second.begin(),
                    pair.second.end(),
                    OrderComparer<GraphicShape *>());}
    }

    void Scene::sort_actor_maps() {
//        std::sort(actor_order.begin(),
//                actor_order.end(),
//                OrderComparer<Actor *>());
        for (auto &pair: shape_actor_map) {
            std::sort(pair.second.begin(),
                    pair.second.end(),
                    OrderComparer<Actor *>());}
    }

    void Scene::sort_blending_actors() {
        std::sort(blending_actors.begin(),
                  blending_actors.end(),
                  _actor_blending_comparer);
    }

    void Scene::collect_blending_actors() {
        blending_actors.clear();
        blending_actor_shape_material_map.clear();
        for (auto &ord_mat: renderlayer_material_map[RenderLayer::BLENDING])
        {
            for(auto shp: material_shape_map[ord_mat.second])
            {
                for(auto act: shape_actor_map[shp])
                {
                    blending_actors.push_back(act);
                    blending_actor_shape_material_map[act] = std::pair(shp, ord_mat.second);
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

        float xoffset = x_pos - scene->cam_lastX;
        float yoffset = scene->cam_lastY - y_pos;

        scene->cam_lastX = x_pos;
        scene->cam_lastY = y_pos;

        scene->get_camera()->ProcessMouseMovement(xoffset, yoffset);
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Scene::get_instance()->get_camera()->ProcessMouseScroll(yoffset);
    }

}