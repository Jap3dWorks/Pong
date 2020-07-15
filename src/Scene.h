#ifndef SCENE_H
#define SCENE_H

#include "Actor.h"

// #include "Camera.h"
#include "Shape.h"
#include "Collider.h"
#include "Render.h"
#include "Lights.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include <map>
#include <functional>



namespace Pong {

    void mouse_callback(GLFWwindow*, double, double);
    void scroll_callback(GLFWwindow*, double, double);

    class Scene
    {
    // singleton class
    public:
        static const int POINT_LIGHTS = 5;

        float cam_lastX = (float)Render::SCR_WIDTH / 2.0;
        float cam_lastY = (float)Render::SCR_HEIGHT / 2.0;
        bool cam_firstMouse = true;

    private:
        static Scene* instance;  // singleton static
        // point lights pointers
        PointLight* _pointLights = new PointLight[POINT_LIGHTS];
        // directional light
        DirectionalLight* _directionalLight = new DirectionalLight;

        // camera pointer
        Camera* _camera = new Camera("default_cam", glm::vec3(0.f, 0.f, 5.f));

        //private methods
        Scene();

        void _process_node(aiNode * node,
                           const aiScene *& scene,
                           std::vector<Mesh*>& out_result);
        static Mesh * _process_mesh(const aiMesh *& mesh);

    public:
        virtual ~Scene();

        static Scene* get_instance();

        std::map<std::string, Actor*> actor_map;
        std::map<std::string, Material*> material_map;
        std::map<std::string, Collider*> collider_map;
        std::map<std::string, Shape*> shape_map;
        std::map<std::string, Shader*> shader_map;
        std::map<std::string, Texture*> textures_map;

        // Material Actors map, use this map to draw objects by material.
        // TODO: fill this map
        std::map<Material*, std::vector<Actor*>> material_meshes_map;

        PointLight& get_point_light(int id) const;

        DirectionalLight* get_directional_light() const;

        Shader* create_shader(const std::string& name,
            const GLchar* vertex_shader,
            const GLchar* fragment_shader,
            const GLchar* geometry_shader = nullptr);

        Shader* get_shader(const std::string& name) const;

        // create a material and save it in _materialMap
        Material* create_material(const std::string& name,
                                  Shader* shader,
                                  std::vector<Texture*> textures);

        // get a material by its name
        Material* get_material(const std::string& name) const;

        // create texture
        Texture* create_texture(const std::string& name,
                                const std::string& path,
                                std::string texture_type);

        Texture* get_texture(const std::string& name) const;

        /**create_texture Sky box overload.*/
        Texture* create_texture(const std::string& name,
                                const std::string& texture_type,
                                const std::string& right,
                                const std::string& left,
                                const std::string& top,
                                const std::string& bottom,
                                const std::string& front,
                                const std::string& back);

        // Create an actor and save in _actorMap
        template<typename T>
        T* create_actor(std::string name)
        {
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
                // if key is already in map, delete prev ptr and add the new ptr
                return nullptr;
            }
        }

        // get an actor by its name
        Actor* get_actor(const std::string& name) const;

        // Create a collider, template you must specify the collider type
        template<typename T>
        T* create_collider(std::string name)
        {
            if (!std::is_base_of<Collider, T>::value) {
                return nullptr;
            }
            if (collider_map.find(name) == collider_map.end())
            {
                T* c_ptr = new T(name);  // create collider ptr
                collider_map[name] = static_cast<Collider*>(c_ptr);
                return c_ptr;
            }
            else {
                return nullptr;
            }
        }

        // get a collider by its name
        Collider* get_collider(const std::string& name) const;

        // get camera ptr
        Camera* get_camera() const;

        template<typename T, typename... Args>
        T* create_shape(std::string name, Args&&... args)
        {
            if (!std::is_base_of<Shape, T>::value)
                return nullptr;
            if (shape_map.find(name) == shape_map.end())
            {
                T* s_ptr = new T(name, std::forward<Args>(args)...);
                // store shape pointer in internal level data
                shape_map[name] = static_cast<Shape*>(s_ptr);
                return s_ptr;
            }
            else
                // if shape exists in the map, return ptr to shape
                return static_cast<T*>(shape_map[name]);
        }
        Shape* get_shape(const std::string& name) const;

        int import_model(const std::string& model_path, Actor *& actor);

    };
}
#endif // !SCENE_H
