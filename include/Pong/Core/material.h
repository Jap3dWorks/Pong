#ifndef PONG_MATERIAL_H
#define PONG_MATERIAL_H

#include "Pong/Core/shader.h"
#include "Pong/Core/core_vals.h"
#include <stb_image.h>

#include <iostream>
#include <utility>
#include <vector>
#include <map>

namespace Pong {
    class Render;
    class Scene;
}

namespace Pong {
    class Texture {
    protected:
        std::string _name;
        unsigned int _texture_id{};
        std::string _path;
        std::string _texture_type;
        unsigned int _gl_bind_type = GL_TEXTURE_2D;

    public:
        _P_STATIC _P_INLINE unsigned int load_texture(const char *path, const bool &gammaCorrection) {
            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
            if (data) {
                GLenum internalFormat;
                GLenum dataFormat = GL_RED;
                if (nrComponents == 1) dataFormat = internalFormat = GL_RED;
                else if (nrComponents == 3) {
                    internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                    dataFormat = GL_RGB;
                } else if (nrComponents == 4) {
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
            } else {
                LOG_WARNING("Texture failed to load at path: " << path);
                stbi_image_free(data);
            }

            return textureID;
        }

        _P_EXPLICIT Texture(std::string  name, std::string texture_type):
        _name(std::move(name)), _texture_type(std::move(texture_type))
        {}

        Texture(std::string name, const std::string &path, std::string texture_type) :
                _name(std::move(name)), _path(path), _texture_type(std::move(texture_type)) {
            _texture_id = load_texture(path.c_str(), false);
            _gl_bind_type = GL_TEXTURE_2D;
        }

        virtual ~Texture() = default;

        _P_NODISCARD unsigned int get_id() const { return _texture_id; }
        _P_NODISCARD unsigned int get_gl_bind_type() const {return _gl_bind_type;}
        std::string get_path() { return _path; }
        std::string get_texture_type() { return _texture_type; }
        std::string get_name() { return _name; }
    };

    class Material
    {
    private:
        bool _is_setup = false;
        std::map<std::string, float> _float_params;
        std::map<std::string, int> _int_params;
        std::map<std::string, glm::vec3> _vec3_params;
        std::map<std::string, glm::mat4> _mat4_params;

        void _setup_material() {
            if (_is_setup)
                return;

            // configure Shader for the material
            for (int i = 0; i < _textures.size(); i++) {
                // assign an id to each texture slot
                _shader->setInt(_textures[i]->get_texture_type(), i);
            }

            _is_setup = true;
        }

    protected:
        Shader* _shader;
        std::vector<Texture*> _textures;
        std::string _name;

    public:
        unsigned int order{50};

        Material(std::string name, Shader* shader, std::vector<Texture*> textures):
            _name(std::move(name)),
            _shader(shader),
            _textures(std::move(textures)) {
            _setup_material();
        }

        virtual ~Material() = default;

        _P_NODISCARD Shader* get_shader() const {
            return _shader;
        }

        // stared Parameters that not vary each frame,
        // it can be a roughness value.
        void update_params() {
            // set Shader parametters
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

        // TODO: resolve with a template?
        void set_param(const std::string& param, float value) {
            auto it = _float_params.find(param);
            if (it != _float_params.end())
                it->second = value;
            else
                _float_params.insert(std::pair<std::string, float>(param, value));
        }

        void set_param(const std::string& param, glm::vec3 value) {
            auto it = _vec3_params.find(param);
            if (it != _vec3_params.end())
                it->second = value;
            else
                _vec3_params.insert(std::pair<std::string, glm::vec3>(param, value));
        }

        void set_param(const std::string& param, int value)
        {
            auto it = _int_params.find(param);
            if (it != _int_params.end())
                it->second = value;
            else
                _int_params.insert(std::pair<std::string, int>(param, value));
        }


        void set_param(const std::string& param, glm::mat4 value) {
            auto it = _mat4_params.find(param);
            if (it != _mat4_params.end())
                it->second = value;
            else
                _mat4_params.insert(std::pair<std::string, glm::mat4>(param, value));
        }

        virtual void update_shader(Render*, Scene*)
        {
            update_params();
        }

        _P_NODISCARD std::string get_name() _P_CONST
        { return _name; }

        virtual void use() {
            // bind Shader
            _shader->use();

            // bing Textures texture
            for (unsigned int i =0; i < _textures.size(); i++)
            {
                // Active texture Shader slot
                glActiveTexture(GL_TEXTURE0 + i);
                // bind texture ID
                // e.g  GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
                glBindTexture(_textures[i]->get_gl_bind_type(),
                              _textures[i]->get_id());
            }
        }

        /// unbind Textures
        virtual void end_use() {
            // unbind texture objects
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    };
}

#endif // !PONG_MATERIAL_H