#ifndef SCENE_H
#define SCENE_H

#include "Pong/core/core_vals.h"
#include "Pong/core/actor.h"
#include "Pong/core/material.h"
#include "Pong/core/graphic_shape.h"
#include "Pong/core/collider.h"
#include "Pong/core/lights.h"
#include "Pong/core/render.h"

#include "Pong/core/data_comparers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <cassert>
#include <iostream>
#include <functional>
#include <vector>
#include <set>
#include <map>

namespace Pong {
    // TODO: move mouse callbacks
    void mouse_callback(GLFWwindow*, double, double);
    void scroll_callback(GLFWwindow*, double, double);

    class Scene {
    // singleton class
    public:
        // TODO: last frame data in a struct
        float cam_lastX = 0.f;
        float cam_lastY = 0.f;
        bool cam_firstMouse = true;

    private:
        using OrderMatPtrPair = std::pair<uint32_t, Material*>;
        using OrderMatPtrComparer = OrderComparer<std::pair<uint32_t, Material*>>;

    private:
        static Scene* instance;  // singleton static
        std::vector<PointLight> _point_lights;

        // directional light
        std::vector<DirectionalLight> _directional_lights{{}};

        // camera_ptr pointer
        std::unique_ptr<ACamera> _camera = std::make_unique<ACamera>(
                "default_cam",
                glm::vec3(0.f, 0.f, 5.f)
        );

        ActorCameraDistanceComparer _actor_blending_comparer =
                ActorCameraDistanceComparer(_camera.get());

    private:
        //private methods
        Scene();

    public:
        std::map<std::string, Actor*> actor_map;
        std::map<std::string, Material*> material_map;
        std::map<std::string, Collider*> collider_map;
        std::map<std::string, std::unique_ptr<GraphicShape>> shape_map;
        std::map<std::string, Shader*> shader_map;
        std::map<std::string, Texture*> textures_map;

        // blending actors ordered by distance to Render camera_ptr
        std::vector<Actor*>blending_actors;
        std::map<Actor*, std::pair<GraphicShape*, Material*>> blending_actor_shape_material_map;

        std::map<RenderLayer,
                std::multiset<OrderMatPtrPair, OrderMatPtrComparer>
        > renderlayer_material_map;

        std::map<Material*, std::vector<GraphicShape*>> material_shape_map;
        std::map<GraphicShape*, std::vector<Actor*>> shape_actor_map;

    public:
        virtual ~Scene();
        static Scene* get_instance();

        void collect_blending_actors();
        void assign_layer(const RenderLayer& rlay, Material* material, uint32_t order=0) {
            renderlayer_material_map[rlay].insert({order, material});
        }

        void assign_material(Material*, GraphicShape*);
        void assign_shape(GraphicShape*, Actor*);

//        void sort_materials();
        void sort_shapes_maps();
        void sort_actor_maps();

        void sort_blending_actors();

        _P_NODISCARD std::vector<PointLight>& get_point_lights() {
            return _point_lights;
        }

        _P_NODISCARD std::vector<DirectionalLight>& get_directional_lights() {
            return _directional_lights;
        }

        Shader *create_shader(
                const std::string &name,
                const GLchar *shader_path);

        _P_NODISCARD Shader* get_shader(const std::string& name) const;

        // create a material and save it in _materialMap
        template<typename T>
        T *create_material(const std::string &name,
                           Shader *shader,
                           const TextureUniformVector &textures,
                           const RenderLayer &render_layer = RenderLayer::BASE) {

            if (!std::is_base_of<Material, T>::value)
                return nullptr;

            if (material_map.find(name) == material_map.end()) {
                auto *m_ptr = new T(name, shader, textures);
                material_map[name] = static_cast<Material *>(m_ptr);
//                material_order.push_back(m_ptr);

                assign_layer(render_layer, m_ptr);

                return m_ptr;

            } else {
                return nullptr;
            }
        }

        // get a material by its name
        _P_NODISCARD Material* get_material(const std::string& name) const;

        // create texture
        Texture* create_texture(const std::string& name,
                                const std::string& path);

        _P_NODISCARD Texture* get_texture(const std::string& name) const;

        /**create_texture Sky box overload.*/
        Texture* create_texture(const std::string& name,
                                const std::string& right,
                                const std::string& left,
                                const std::string& top,
                                const std::string& bottom,
                                const std::string& front,
                                const std::string& back)  {
            if (textures_map.find(name) == textures_map.end())
            {
                auto* sb_ptr = new SkyBox(
                        name,
                        right, left, top,
                        bottom,front, back);

                textures_map[name] = sb_ptr;

            }
            return textures_map[name];
        }

        template<typename T>
        T* create_actor(const std::string& name) {
            if (!std::is_base_of<Actor, T>::value)
                return nullptr;

            if (actor_map.find(name) == actor_map.end())
            {
                T* a_ptr = new T(name);
                actor_map[name] = static_cast<Actor*>(a_ptr);
                return a_ptr;
            }
            else
            {
                return nullptr;
            }
        }

        // get an Actor by its name
        _P_NODISCARD Actor* get_actor(const std::string& name) const;

        // Create a Collider, template you must specify the Collider type
        template<typename T>
        T* create_collider(const std::string& name) {
            if (!std::is_base_of<Collider, T>::value) {
                return nullptr;
            }
            if (collider_map.find(name) == collider_map.end())
            {
                T* c_ptr = new T(name);  // create Collider ptr
                collider_map[name] = static_cast<Collider*>(c_ptr);
                return c_ptr;
            }
            else {
                return nullptr;
            }
        }

        // get a Collider by its name
        _P_NODISCARD Collider* get_collider(const std::string& name) const;

        // get camera_ptr ptr
        _P_NODISCARD ACamera* get_camera() const
        {return _camera.get();}

        template<typename T, typename... Args>
        _P_INLINE T* create_shape(const std::string& name, Args&&... args) {
            #ifdef NDEBUG
            #else
            auto is_base = is_base_of_v<GraphicShape, T>;
            assert(is_base);
            #endif


            auto ptr = new T(std::forward<Args>(args)...);
            shape_map[name] = std::unique_ptr<GraphicShape>(
                    static_cast<GraphicShape*>(ptr)
                    );

            return static_cast<T*>(shape_map[name].get());
        }

        _P_NODISCARD _P_INLINE bool contains_shape(const std::string& name) const {
            return shape_map.find(name) != shape_map.end();
        }

        _P_NODISCARD _P_INLINE GraphicShape* get_shape(const std::string& name) const {
            assert(shape_map.find(name) != shape_map.end());
            return shape_map[name];
        }
    };
}
#endif // !SCENE_H
