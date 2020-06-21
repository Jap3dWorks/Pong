#ifndef SCENE_H
#define SCENE_H

//#include "pongTypes.h"

#include "Actor.h"

//#include "Camera.h"
#include "Shape.h"
#include "Collider.h"
#include "Render.h"

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include <map>
#include <functional>



namespace Pong {
    // todo move lights to its own file
    struct PointLight
    {
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);

    };

    struct DirectionalLight {
        glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f);
        glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 ambient = glm::vec3(0.f, 0.f, 0.f);
    };

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

    public:
        virtual ~Scene();

        static Scene* getInstance();

        // std::map<std::string, std::vector> shader_meshes_map;

        std::map<std::string, Actor*> actor_map;
        std::map<std::string, Material*> material_map;
        std::map<std::string, Collider*> collider_map;
        std::map<std::string, Shape*> shape_map;
        std::map<std::string, Shader*> shader_map;
        std::map<std::string, Texture*> textures_map;

        PointLight& get_point_light(int id);

        DirectionalLight* getDirectionalLight();

        void material_to_shape(Material*, Actor*);

        Shader* create_shader(std::string name,
            const GLchar* vertex_shader,
            const GLchar* fragment_shader,
            const GLchar* geometry_shader = nullptr);

        Shader* get_shader(std::string name);

        // create a material and save it in _materialMap
        Material* createMaterial(std::string name,
            Shader* shader,
            std::vector<Texture*> textures);

        // get a material by its name
        Material* getMaterial(std::string name);

        // create texture
        Texture* create_texture(std::string name,
            std::string path,
            std::string texture_type);

        Texture* get_texture(std::string name);

        // Create an actor and save in _actorMap
        template<typename T>
        T* createActor(std::string name)
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
        Actor* getActor(std::string name);

        // create a shape and save it in _shapeMap
        template<typename T, typename... Args>
        T* createShape(std::string name, Args&&... args)
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
        // get shape
        Shape* getShape(std::string name);

        // Create a collider, template you must specify the collider type
        template<typename T>
        T* createCollider(std::string name)
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
        Collider* getCollider(std::string name);

        // get camera ptr
        Camera* getCamera();

    }; // end Scene
}
#endif // !SCENE_H
