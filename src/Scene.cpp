#include "Scene.h"
#include "logger.h"

Pong::Scene* Pong::Scene::instance = 0;
namespace Pong{
    Scene::Scene()
    {
        // start Render
        Render* render = Render::getInstance();
        // create callbacks
        glfwSetCursorPosCallback(render->getWindow(), Pong::mouse_callback);
        glfwSetScrollCallback(render->getWindow(), scroll_callback);

        //for (int i = 0; i < POINT_LIGHTS; i++)
        //{
        //	_pointLights[i] = new PointLight;
        //}
    }

    Scene::~Scene()
    {
        LOG_DEBUG("Call scene destructor");
        delete _directionalLight;
        delete[] _pointLights;

        delete _camera;

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

        auto sit = shape_map.begin();
        for (int i = 0; i < shape_map.size(); i++)
        {
            delete std::next(sit, i)->second;
        }

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

    void Scene::material_to_shape(Material* material, Actor* actor)
    {
        // posibly material set in shape class has more sense
        actor->setMaterial(material);

        // shader_meshes_map;
    }

    Scene* Pong::Scene::getInstance()
    {
        if (!instance)
        {
            instance = new Scene;
        }
        return instance;
    }

    PointLight& Pong::Scene::get_point_light(int id)
    {
        if (id < POINT_LIGHTS)
            return _pointLights[id];
        else
            return _pointLights[0];
    }

    DirectionalLight* Pong::Scene::getDirectionalLight()
    {
        return _directionalLight;
    }

    Camera* Pong::Scene::getCamera()
    {return _camera;}

    // --Actors--
    //template<typename T>
    //T* Scene::createActor(std::string name)
    //{
    //	if (!std::is_base_of<Actor, T>::value)
    //		return nullptr;

    //	if (_actorMap.find(name) == _actorMap.end())
    //	{
    //		T* a_ptr = new T(name);
    //		_actorMap[name] = static_cast<Actor*>(a_ptr);
    //		return a_ptr;
    //	}
    //	else
    //	{
    //		// if key is already in map, delete prev ptr and add the new ptr
    //		return nullptr;
    //	}
    //}

    Actor* Scene::getActor(std::string name)
    {
        if (actor_map.find(name) != actor_map.end())
        {
            return actor_map[name];
        }
        else
        {
            return nullptr;
        }

    }

    // -- Colliders--

    Collider* Scene::getCollider(std::string name)
    {
        if (collider_map.find(name) != collider_map.end())
        {
            return collider_map[name];
        }
        else
            return nullptr;
    }


    Shape* Scene::getShape(std::string name)
    {
        if (shape_map.find(name) != shape_map.end())
        {
            return shape_map[name];
        }
        else
            return nullptr;
    }

    Shader* Scene::create_shader(std::string name, const GLchar* vertex_shader,
        const GLchar* fragment_shader, const GLchar* geometry_shader)
    {
        if (shader_map.find(name) == shader_map.end())
        {
            Shader* ptr = new Shader(vertex_shader, fragment_shader, geometry_shader);
            shader_map[name] = ptr;
            return ptr;
        }
        else
            return shader_map[name];
    }

    Shader* Scene::get_shader(std::string name)
    {
        if (shader_map.find(name) != shader_map.end())
        {
            return shader_map[name];
        }
        else
            return nullptr;
    }

    // --Materials--
    Material* Scene::createMaterial(std::string name,
        Shader* shader,
        std::vector<Texture*> textures)
    {
        if (material_map.find(name) == material_map.end())
        {
            Material* m_ptr = new Material(name, shader, textures);
            material_map[name] = m_ptr;

            return material_map[name];
        }
        else {
            return material_map[name];
        }
    }

    Material* Scene::getMaterial(std::string name)
    {
        if (material_map.find(name) != material_map.end())
        {
            return material_map[name];
        }
        else
            return nullptr;
    }

    // --textures--
    Texture* Scene::create_texture(std::string name,
        std::string path, std::string texture_type)
    {
        if (textures_map.find(name) == textures_map.end())
        {
            Texture* t_ptr = new Texture(name, path, texture_type);
            textures_map[name] = t_ptr;

            return textures_map[name];
        }
        else
        {
            return textures_map[name];
        }
    }

    Texture* Scene::get_texture(std::string name)
    {
        if (textures_map.find(name) != textures_map.end())
        {
            return textures_map[name];
        }
        else
            return nullptr;
    }

    // callback funtions
    // -----------------
    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Scene* scene = Scene::getInstance();
        if (scene->cam_firstMouse)
        {
            scene->cam_lastX = xpos;
            scene->cam_lastY = ypos;
            scene->cam_firstMouse = false;
        }

        float xoffset = xpos - scene->cam_lastX;
        float yoffset = scene->cam_lastY - ypos;

        scene->cam_lastX = xpos;
        scene->cam_lastY = ypos;

        scene->getCamera()->ProcessMouseMovement(xoffset, yoffset);
    }



    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Scene::getInstance()->getCamera()->ProcessMouseScroll(yoffset);
    }

}