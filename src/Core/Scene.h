#ifndef SCENE_H
#define SCENE_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Actor.h"
#include "../Graphic/Material.h"
#include "Shape.h"
#include "Collider.h"
#include "../Graphic/Lights.h"
#include "Render.h"

#include "DataComparers.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <set>
#include <map>
#include <functional>



namespace Pong {

    void mouse_callback(GLFWwindow*, double, double);
    void scroll_callback(GLFWwindow*, double, double);

    class Scene
    {
    // singleton class
    public:
        static const int POINT_LIGHTS_COUNT = 5;

        // point lights pointers
        PointLight point_lights_array[POINT_LIGHTS_COUNT];

        float cam_lastX = (float)Render::SCR_WIDTH / 2.0;
        float cam_lastY = (float)Render::SCR_HEIGHT / 2.0;
        bool cam_firstMouse = true;

    private:
        static Scene* instance;  // singleton static

        // directional light
        DirectionalLight* _directionalLight = new DirectionalLight;

        // camera_ptr pointer
        ACamera* _camera = new ACamera("default_cam",
                glm::vec3(0.f, 0.f, 5.f));
        ActorCameraDistanceComparer _actor_blending_comparer =
                ActorCameraDistanceComparer(_camera);

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

        std::vector<Material*> material_order;
        std::vector<Shape*> shape_order;
        std::vector<Actor*> actor_order;

        // blending actors ordered by distance to render camera_ptr
        std::vector<Actor*>blending_actors;
        std::map<Actor*, std::pair<Shape*, Material*>> blending_actor_shape_material_map;
        void collect_blending_actors();

        // I have the problem of a shape in two different materials.
        // shape actors will draw twice all materials in this case.
        std::map<RenderLayer, std::vector<Material*>> renderlayer_material_map;
        std::map<Material*, std::vector<Shape*>> material_shape_map;
        std::map<Shape*, std::vector<Actor*>> shape_actor_map;

        void assign_layer(const RenderLayer&, Material*);

        void assign_material(Material*, Shape*);
        void assign_shape(Shape*, Actor*);

        void sort_materials();
        void sort_shapes_maps();
        void sort_actor_maps();

        void sort_blending_actors();

        [[nodiscard]] PointLight& get_point_light(int id);

        [[nodiscard]] DirectionalLight* get_directional_light() const;

        Shader* create_shader(const std::string& name,
            const GLchar* vertex_shader,
            const GLchar* fragment_shader,
            const GLchar* geometry_shader = nullptr);

        [[nodiscard]] Shader* get_shader(const std::string& name) const;

        // create a material and save it in _materialMap
        template<typename T>
        T *create_material(const std::string &name,
                           Shader *shader,
                           const std::vector<Texture *> &textures,
                           const RenderLayer &render_layer = RenderLayer::BASE)
        {
            if (!std::is_base_of<Material, T>::value)
                return nullptr;

            if (material_map.find(name) == material_map.end()) {
                auto *m_ptr = new T(name, shader, textures);
                material_map[name] = static_cast<Material *>(m_ptr);
                material_order.push_back(m_ptr);

                assign_layer(render_layer, m_ptr);

                return m_ptr;

            } else {
                return nullptr;
            }
        }

        // get a material by its name
        [[nodiscard]] Material* get_material(const std::string& name) const;

        // create texture
        Texture* create_texture(const std::string& name,
                                const std::string& path,
                                std::string texture_type);

        [[nodiscard]] Texture* get_texture(const std::string& name) const;

        /**create_texture Sky box overload.*/
        Texture* create_texture(const std::string& name,
                                const std::string& texture_type,
                                const std::string& right,
                                const std::string& left,
                                const std::string& top,
                                const std::string& bottom,
                                const std::string& front,
                                const std::string& back);

        template<typename T>
        T* create_actor(const std::string& name)
        {
            if (!std::is_base_of<Actor, T>::value)
                return nullptr;

            if (actor_map.find(name) == actor_map.end())
            {
                T* a_ptr = new T(name);
                actor_map[name] = static_cast<Actor*>(a_ptr);
                actor_order.push_back(a_ptr);
                return a_ptr;
            }
            else
            {
                return nullptr;
            }
        }

        // get an actor by its name
        [[nodiscard]] Actor* get_actor(const std::string& name) const;

        // Create a collider, template you must specify the collider type
        template<typename T>
        T* create_collider(const std::string& name)
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
        [[nodiscard]] Collider* get_collider(const std::string& name) const;

        // get camera_ptr ptr
        [[nodiscard]] ACamera* get_camera() const;

        template<typename T, typename... Args>
        T* create_shape(const std::string& name, Args&&... args)
        {
            if (!std::is_base_of<Shape, T>::value)
                return nullptr;
            if (shape_map.find(name) == shape_map.end())
            {
                T* s_ptr = new T(name, std::forward<Args>(args)...);
                // store shape pointer in internal level data
                shape_map[name] = static_cast<Shape*>(s_ptr);
                shape_order.push_back(s_ptr);
                return s_ptr;
            }
            else
                // if shape exists in the map, return ptr to shape
                return static_cast<T*>(shape_map[name]);
        }
        [[nodiscard]] Shape* get_shape(const std::string& name) const;

        int import_model(const std::string& model_path, Actor *& actor);

    };
}
#endif // !SCENE_H
