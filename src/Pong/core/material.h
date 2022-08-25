#ifndef PONG_MATERIAL_H
#define PONG_MATERIAL_H

#include "Pong/core/shader.h"
#include "Pong/core/core_vals.h"
#include "Pong/core/texture.h"
#include "Pong/registers/reg_id.h"

#include <stb/stb_image.h>

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>

namespace pong {
    class Render;
    class Map;
}

namespace pong {
    using TextureUniformVector = std::vector<std::pair<std::string, Texture*>>;

    template<typename T>
    using param_data = std::unordered_map<std::string, T>;

    struct Material {
        RegId shader;

        param_data<float> float_params;
        param_data<int> int_params;
        param_data<glm::vec3> vec3_params;
        param_data<glm::mat4> mat4_params;

        param_data<RegId> textures;
    };


    class MaterialController {
    protected:
        using super = MaterialController;

    private:
        bool _is_setup = false;
        param_data<float> _float_params;
        param_data<int> _int_params;
        param_data<glm::vec3> _vec3_params;
        param_data<glm::mat4> _mat4_params;

        void _setup_material() {
            if (!_is_setup) {
                // configure shader for the material
                for (int i = 0; i < _textures.size(); i++) {
                    // assign an id to each texture slot
                    _shader->set_int(
                            _textures[i].first, i
                            );
                }
            }
            _is_setup = true;
        }

    protected:
        Shader* _shader;
        TextureUniformVector _textures{};

    public:
        uint32_t order{50};

        explicit MaterialController(Shader * shader) : _shader(shader) {
            _setup_material();
        }

        MaterialController(Shader *shader,
                           TextureUniformVector textures) :
                _shader(shader),
                _textures(std::move(textures)) {
            _setup_material();
        }

        virtual ~MaterialController() = default;

        _P_NODISCARD Shader* get_shader() const {
            return _shader;
        }

        // stared Parameters that not vary each frame,
        // it can be a roughness value.
        void update_params() {
            // set shader parametters
            for (auto &p : _float_params) // floats
            {
                _shader->set_float(p.first, p.second);
            }
            for (auto &p : _int_params) // ints
            {
                _shader->set_int(p.first, p.second);
            }
            for (auto &p : _vec3_params) // vec3
            {
                _shader->set_vec3(p.first, p.second);
            }
            for (auto &p : _mat4_params) //mat4
            {
                _shader->set_mat4(p.first, p.second);
            }
        }

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

        virtual void update_shader(Render*, Map*) {
            update_params();
        }

        virtual void use() {
            // bind shader
            _shader->use();

            // bing Textures texture
            for (unsigned int i =0; i < _textures.size(); i++)
            {
                // Active texture shader slot
                glActiveTexture(GL_TEXTURE0 + i);
                // bind texture ID
                // e.g  GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
                glBindTexture(_textures[i].second->get_gl_bind_type(),
                              _textures[i].second->get_id());
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