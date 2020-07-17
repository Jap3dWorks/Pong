#include "Material.h"
#include "logger.h"
#include "Scene.h"

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utility>

namespace Pong {

    Texture::Texture(std::string name, const std::string& path, std::string texture_type) :
        _name(std::move(name)), _path(path), _texture_type(std::move(texture_type))
    {
        _texture_id = load_texture(path.c_str(), false);
        _gl_bind_type = GL_TEXTURE_2D;
    }

    Texture::~Texture() = default;

    unsigned int Texture::load_texture(const char *path, const bool &gammaCorrection)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum internalFormat;
            GLenum dataFormat;
            if (nrComponents == 1)
                dataFormat = internalFormat = GL_RED;
            else if (nrComponents == 3)
            {
                internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                dataFormat = GL_RGB;
            }
            else if (nrComponents == 4)
            {
                internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                dataFormat = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         internalFormat,
                         width,
                         height,
                         0,
                         dataFormat,
                         GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            LOG_WARNING("Texture failed to load at path: " << path);
            stbi_image_free(data);
        }

        return textureID;
    }

    Material::Material(std::string name, Shader* shader, std::vector<Texture*> textures):
        _name(std::move(name)), _shader(shader), _textures(std::move(textures))
    {
        _setup_material();
    }

    Material::~Material() = default;

    void Material::_setup_material()
    {
        if (_is_setup)
            return;

        // configure shader for the material
        for (int i = 0; i < _textures.size(); i++)
        {
            // assign an id to each texture slot
            _shader->setInt(_textures[i]->get_texture_type(), i);
        }

        _is_setup = true;
    }

    void Material::use()
    {
        // bing textures texture
        for (unsigned int i =0; i < _textures.size(); i++)
        {
            // Active texture shader slot
            glActiveTexture(GL_TEXTURE0 + i);
            // bind texture ID
            // e.g  GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
            glBindTexture(_textures[i]->get_gl_bind_type(),
                          _textures[i]->get_id());
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

    void Material::update_shader(const Render *render, const Scene *scene) {
        // bind shader
        _shader->use();

        _shader->setMat4("view", scene->get_camera()->get_view_matrix());
        _shader->setMat4("projection",
                         glm::perspective(glm::radians(scene->get_camera()->Zoom),
                                          (float) Pong::Render::SCR_WIDTH / (float) Pong::Render::SCR_HEIGHT,
                                          0.1f,
                                          10000.f));

        // directional light
        _shader->setVec3("directional.Direction", scene->get_directional_light()->direction);
        _shader->setVec3("directional.Color", scene->get_directional_light()->color);
        _shader->setVec3("directional.Ambient", scene->get_directional_light()->ambient);

        // points lights
        for (unsigned int i = 0; i < Scene::POINT_LIGHTS; ++i) {
            _shader->setVec3("pointLightPositions[" + std::to_string(i) + "]",
                             scene->get_point_light(i).position);
            _shader->setVec3("pointLightColors[" + std::to_string(i) + "]",
                             scene->get_point_light(i).color);
        }
    }

    Shader* Material::get_shader() const
    {
        return _shader;
    }

    // set parameters
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

    unsigned int SkyBox::load_skybox_textures(const std::vector<std::string> &faces) {
        unsigned int texture_id;

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        int width, height, nrChannels;
        for (unsigned int i=0; i < faces.size(); ++i)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                        width, height,
                        0, GL_RGB, GL_UNSIGNED_BYTE, data);

                stbi_image_free(data);
            } else{
                LOG_ERROR("Skybox texture failed to load at path: " << faces[i])
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return texture_id;
    }

}