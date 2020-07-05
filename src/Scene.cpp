#include "Scene.h"

#include <utility>
#include "logger.h"
#include "Lights.h"


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

    Scene* Pong::Scene::get_instance()
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

    Camera* Pong::Scene::get_camera()
    {return _camera;}

    // --Actors--
    //template<typename T>
    //T* Scene::create_actor(std::string name)
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

    Actor* Scene::get_actor(const std::string& name)
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

    Collider* Scene::getCollider(const std::string& name)
    {
        if (collider_map.find(name) != collider_map.end())
        {
            return collider_map[name];
        }
        else
            return nullptr;
    }


    Shape* Scene::get_shape(const std::string& name)
    {
        if (shape_map.find(name) != shape_map.end())
        {
            return shape_map[name];
        }
        else
            return nullptr;
    }

    Shader* Scene::create_shader(const std::string& name, const GLchar* vertex_shader,
        const GLchar* fragment_shader, const GLchar* geometry_shader)
    {
        if (shader_map.find(name) == shader_map.end())
        {
            auto* ptr = new Shader(vertex_shader, fragment_shader, geometry_shader);
            shader_map[name] = ptr;
            return ptr;
        }
        else
            return shader_map[name];
    }

    Shader* Scene::get_shader(const std::string& name)
    {
        if (shader_map.find(name) != shader_map.end())
        {
            return shader_map[name];
        }
        else
            return nullptr;
    }

    // --Materials--
    Material* Scene::create_material(const std::string& name,
                                     Shader* shader,
                                     std::vector<Texture*> textures)
    {
        if (material_map.find(name) == material_map.end())
        {
            auto* m_ptr = new Material(name, shader, std::move(textures));
            material_map[name] = m_ptr;

            return material_map[name];
        }
        else {
            return material_map[name];
        }
    }

    Material* Scene::get_material(const std::string& name)
    {
        if (material_map.find(name) != material_map.end())
        {
            return material_map[name];
        }
        else
            return nullptr;
    }

    // --textures--
    Texture* Scene::create_texture(const std::string& name,
                                   const std::string& path, std::string texture_type)
    {
        if (textures_map.find(name) == textures_map.end())
        {
            auto* t_ptr = new Texture(name, path, std::move(texture_type));
            textures_map[name] = t_ptr;

            return textures_map[name];
        }
        else
        {
            return textures_map[name];
        }
    }

    Texture* Scene::create_texture(const std::string& name,
                                   const std::string& right,
                                   const std::string& left,
                                   const std::string& top,
                                   const std::string& bottom,
                                   const std::string& front,
                                   const std::string& back){
        if (textures_map.find(name) == textures_map.end())
        {
            auto* sb_ptr = new SkyBox(
                    name,
                    right, left, top,
                    bottom,front, back);

            textures_map[name] = sb_ptr;

            return textures_map[name];
        }
        else return textures_map[name];
    }

    Texture* Scene::get_texture(const std::string& name)
    {
        if (textures_map.find(name) != textures_map.end())
        {
            return textures_map[name];
        }
        else
            return nullptr;
    }

    // import model methods
    /**Import model from a file like fbx or obj, the shapes that compose the model,
      Will be added to the scene.
      Returns: Number of shapes imported.*/
    int Scene::import_model(const std::string& model_path, Actor *& actor)
    {

        // read file
        Assimp::Importer importer;
        const aiScene *ai_scene = importer.ReadFile(
                model_path,
                aiProcess_Triangulate | aiProcess_CalcTangentSpace);

        // check for errors
        if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
        {
            LOG_ERROR("ASSIMP:: " << importer.GetErrorString());
            throw MeshException(importer.GetErrorString());
        }

        // directory path
        std::string directory = model_path.substr(0,
                                                  model_path.find_last_of('/'));

        std::string file_name = model_path.substr(model_path.find_last_of('/') + 1);

        std::vector<Mesh *> result;
        _process_node(ai_scene->mRootNode, ai_scene, result);

        for(auto shape: result)
        {
            shape->set_name(file_name + ":" + shape->get_name());
            shape_map[shape->get_name()] = shape;
            actor->add_shape(shape);
        }

        return result.size();
    }

    void Scene::_process_node(aiNode * node,
                              const aiScene *& scene,
                              std::vector<Mesh*>& out_result)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            out_result.push_back(_process_mesh(mesh));
        }

        for (unsigned int i=0; i < node->mNumChildren; i++)
        {
            _process_node(node->mChildren[i], scene, out_result);
        }
    }

    Mesh * Scene::_process_mesh(const aiMesh *& mesh)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for(unsigned int i=0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex{};
            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            //normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            // texture coordinates
            if (mesh->mTextureCoords[0])  // only one id
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.tex_coords = vec;
            }
            else vertex.tex_coords = glm::vec2 (0.f);
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;

            vertices.push_back(vertex);
        }
        // vertex indices
        for(unsigned int i=0; i<mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j=0; j<face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        return new Mesh(mesh->mName.C_Str(), vertices, indices);
    }

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