#ifndef SCENE_H
#define SCENE_H

#include "Pong/core/core_vals.h"
#include "Pong/core/actor.h"
#include "Pong/core/material.h"
#include "Pong/core/graphic_component.h"
#include "Pong/core/collider.h"
#include "Pong/core/lights.h"
#include "Pong/core/render.h"

#include "Pong/core/data_comparers.h"
#include "utils/type_conditions.h"

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

    class Map {

    public:
        using OrderMatPtrPair = std::pair<uint32_t, Material*>;
        using OrderMatPtrComparer = OrderComparer<OrderMatPtrPair>;

        using OrderShpPtrPair = std::pair<uint32_t, GraphicComponent*>;
        using OrderShpPtrComparer = OrderComparer<OrderShpPtrPair>;

        using OrderActorPtrPair = std::pair<uint32_t, Actor*>;
        using OrderActorPtrComparer = OrderComparer<OrderActorPtrPair>;

        using ShpMatPtrPair = std::pair<GraphicComponent*, Material*>;

        template<typename T>
        using CameraOrderedActors = std::multiset<T*, ActorCameraDistanceComparer>;

    public:
        // TODO: last frame data in a struct
        float cam_lastX = 0.f;
        float cam_lastY = 0.f;
        bool cam_firstMouse = true;

    private:
        inline static std::unique_ptr<Map> instance;
        std::vector<PointLight> _point_lights;

        // directional light
        std::vector<DirectionalLight> _directional_lights{{}};

        // TODO: Camera list or camera map
        // camera_ptr pointer
        Actor* active_camera;

//        std::unique_ptr<Actor> _camera = std::make_unique<ACamera>(
//                "default_cam",
//                glm::vec3(0.f, 0.f, 5.f)
//        );

    public:
        std::map<std::string, Actor*> actor_map;
        std::map<std::string, std::unique_ptr<Material>> material_map;
        std::map<std::string, Collider*> collider_map;
        std::map<std::string, std::unique_ptr<GraphicComponent>> shape_map;
        std::map<std::string, Shader*> shader_map;
        std::map<std::string, std::unique_ptr<Texture>> textures_map;

        std::vector<Actor*> blending_actors;

        std::map<Actor*, ShpMatPtrPair> blending_actor_shape_material_map;

        // Relation maps
        std::map<RenderLayer,
                std::multiset<OrderMatPtrPair, OrderMatPtrComparer>
        > renderlayer_material_map;

        std::map<Material *,
                std::multiset<OrderShpPtrPair, OrderShpPtrComparer>
        > material_shape_map;

        std::map<GraphicComponent*,
                std::multiset<OrderActorPtrPair, OrderActorPtrComparer>
        > shape_actor_map;

    private:

        _P_INLINE void _initialize() {

//            blending_actors = std::multiset<Actor*,
//                            ActorCameraDistanceComparer>(
//                    ActorCameraDistanceComparer(_camera.get())
//                    );
        }

        //private methods
        Map() {
            _initialize();
            // start Render
//            Render* render = Render::get_instance();

            // create callbacks
//            glfwSetCursorPosCallback(render->get_window(), Pong::mouse_callback);
//            glfwSetScrollCallback(render->get_window(), scroll_callback);
        }



    public:
        virtual ~Map();
        static Map* get_instance();

    public:
        CameraOrderedActors<Actor> camera_sorted_blending_actors() {
            CameraOrderedActors<Actor> result(
                    ActorCameraDistanceComparer(
                            _camera.get()
                    ));

            for (auto &actor: blending_actors) {
                result.insert(actor);
            }
            return result;
        }

        void collect_blending_actors();

        void assign_layer(const RenderLayer& rlay, Material* material, uint32_t order=0) {
            renderlayer_material_map[rlay].insert({order, material});
        }

        void assign_material(Material* material,
                             GraphicComponent* shape,
                             uint32_t order=0) {
            material_shape_map[material].insert({order, shape});
        }

        void assign_shape(GraphicComponent* shape,
                          Actor* actor,
                          uint32_t order=0) {
            shape_actor_map[shape].insert({order, actor});
        }

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
        template<CondIsBase<Material> T, typename... Args>
        T *create_material(const std::string &name,
                           const RenderLayer &render_layer = RenderLayer::BASE,
                           Args &&... args) {
            auto ptr = new T(std::forward<Args>(args)...);
            material_map[name] = std::unique_ptr<Material>(
                    static_cast<Material*>(ptr)
            );

            assign_layer(render_layer, ptr);

            return static_cast<T*>(material_map[name].get());
        }

        _P_NODISCARD _P_INLINE bool contains_material(const std::string& name) const {
            return material_map.find(name) != material_map.end();
        }

        _P_NODISCARD _P_INLINE Material* get_material(const std::string& name) const {
            assert(material_map.find(name) != material_map.end());
            return material_map.at(name).get();
        }

        // create texture
        template<CondIsBase<Texture> T, typename ...Args>
        Texture* create_texture(const std::string& name,
                                Args&&... args) {
            auto ptr = new T(std::forward<Args>(args)...);
            textures_map[name] = std::unique_ptr<Texture>(
                    static_cast<Texture*>(ptr)
            );

            return static_cast<T*>(textures_map[name].get());
        }

        _P_NODISCARD bool contains_texture(const std::string& name) {
            return textures_map.find(name) != textures_map.end();
        }

        _P_NODISCARD Texture *get_texture(const std::string &name) const {
            assert(textures_map.find(name) != textures_map.end());
            return textures_map.at(name).get();
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
        _P_NODISCARD Actor* get_camera() const
        {return _camera.get();}

        template<CondIsBase<GraphicComponent> T, typename... Args>
        _P_INLINE T* create_shape(const std::string& name, Args&&... args) {
            auto ptr = new T(std::forward<Args>(args)...);
            shape_map[name] = std::unique_ptr<GraphicComponent>(
                    static_cast<GraphicComponent*>(ptr)
                    );

            return static_cast<T*>(shape_map[name].get());
        }

        _P_NODISCARD _P_INLINE bool contains_shape(const std::string& name) const {
            return shape_map.find(name) != shape_map.end();
        }

        _P_NODISCARD _P_INLINE GraphicComponent* get_shape(const std::string& name) const {
            assert(shape_map.find(name) != shape_map.end());
            return shape_map.at(name).get();
        }
    };
}
#endif // !SCENE_H
