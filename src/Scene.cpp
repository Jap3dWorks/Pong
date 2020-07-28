#include "Scene.h"

#include <utility>
#include "logger.h"


namespace Pong{
    Scene* Scene::instance = nullptr;

    Scene::Scene()
    {
        // start Render
        Render* render = Render::getInstance();
        // create callbacks
        glfwSetCursorPosCallback(render->getWindow(), Pong::mouse_callback);
        glfwSetScrollCallback(render->getWindow(), scroll_callback);
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

    Scene* Scene::get_instance()
    {
        if (!instance)
        {
            instance = new Scene;
        }
        return instance;
    }

    PointLight& Scene::get_point_light(int id) const
    {
        if (id < POINT_LIGHTS)
            return _pointLights[id];
        else
            return _pointLights[0];
    }

    DirectionalLight* Scene::get_directional_light() const
    {
        return _directionalLight;
    }

    ACamera* Scene::get_camera() const
    {return _camera;}

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


    Shape* Scene::get_shape(const std::string& name) const
    {
        if (shape_map.find(name) != shape_map.end())
        {
            return shape_map.at(name);
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

    // --textures--
    Texture* Scene::create_texture(const std::string& name,
                                   const std::string& path,
                                   std::string texture_type)
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
                                   const std::string& texture_type,
                                   const std::string& right,
                                   const std::string& left,
                                   const std::string& top,
                                   const std::string& bottom,
                                   const std::string& front,
                                   const std::string& back){
        if (textures_map.find(name) == textures_map.end())
        {
            auto* sb_ptr = new SkyBox(
                    name, texture_type,
                    right, left, top,
                    bottom,front, back);

            textures_map[name] = sb_ptr;

            return textures_map[name];
        }
        else return textures_map[name];
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
            assign_shape(shape, actor);
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

    void Scene::assign_material(Material * material, Shape * shape) {
        material_shape_map[material].push_back(shape);
    }
    void Scene::assign_shape(Shape * shape, Actor * actor) {
        shape_actor_map[shape].push_back(actor);
    }

    void Scene::sort_materials() {
        std::sort(material_order.begin(), material_order.end(), OrderComparer<Material *>());
    }

    void Scene::sort_shapes_maps() {
        std::sort(shape_order.begin(), shape_order.end(), OrderComparer<Shape *>());
        for (auto &pair: material_shape_map) {
            std::sort(pair.second.begin(), pair.second.end(), OrderComparer<Shape *>());
        }
    }

    void Scene::sort_actor_maps() {
        std::sort(actor_order.begin(), actor_order.end(), OrderComparer<Actor *>());
        for (auto &pair: shape_actor_map) {
            std::sort(pair.second.begin(), pair.second.end(), OrderComparer<Actor *>());
        }
    }

    // TODO: Default scene per asset?
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