#include "Material.h"
#include "Utils.h"
#include "logger.h"

namespace Pong {

    Texture::Texture(std::string name, std::string path, std::string texture_type) :
        _name(name), _path(path), _texture_type(texture_type)
    {
        _Id_texture = loadTexture(path.c_str(), false);
    }

    Texture::~Texture()
    {
    }

    Material::Material(std::string name, Shader* shader, std::vector<Texture*> textures):
        _name(name), _shader(shader), _textures(textures)
    {
        _setup_material();
    }

    Material::~Material()
    {
    }

    void Material::_setup_material()
    {
        if (_is_setup)
            return;

        // configure shader for the material
        for (int i = 0; i < _textures.size(); i++)
        {
            _shader->setInt(_textures[i]->get_texture_type(), i);

        }

        _is_setup = true;
    }

    void Material::use()
    {
        // bind shader
        _shader->use();
        // bing textures texture
        for (unsigned int i =0; i < _textures.size(); i++)
        {
            // Active texture shader slot
            glActiveTexture(GL_TEXTURE0 + i);
            // bind texture ID
            glBindTexture(GL_TEXTURE_2D, _textures[i]->get_id());
        }
        // set shader parametters
        for (auto &p : _float_params) // floats
        {
            _shader->setFloat(p.first, p.second);
        }
        for (auto &p : _int_params) // ints
        {
            _shader->setInt(p.first, p.second);
        }
        for (auto &p : _vec3_params) // vec3
        {
            _shader->setVec3(p.first, p.second);
        }
        for (auto &p : _mat4_params) //mat4
        {
            _shader->setMat4(p.first, p.second);
        }
    }

    void Material::end_use()
    {
        // unbind texture objects
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Shader* Material::get_shader()
    {
        return _shader;
    }

    // set parametters
    void Material::set_param(std::string param, float value)
    {
        auto it = _float_params.find(param);
        if (it != _float_params.end())
            it->second = value;
        else
            _float_params.insert(std::pair<std::string, float>(param, value));
    }
    void Material::set_param(std::string param, glm::vec3 value)
    {
        auto it = _vec3_params.find(param);
        if (it != _vec3_params.end())
            it->second = value;
        else
            _vec3_params.insert(std::pair<std::string, glm::vec3>(param, value));
    }
    void Material::set_param(std::string param, int value)
    {
        auto it = _int_params.find(param);
        if (it != _int_params.end())
            it->second = value;
        else
            _int_params.insert(std::pair<std::string, int>(param, value));
    }
    void Material::set_param(std::string param, glm::mat4 value)
    {
        auto it = _mat4_params.find(param);
        if (it != _mat4_params.end())
            it->second = value;
        else
            _mat4_params.insert(std::pair<std::string, glm::mat4>(param, value));
    }
}