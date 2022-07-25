//
// Created by Jordi on 4/23/2022.
//

#ifndef PONG_DEFAULTMATERIALS_H
#define PONG_DEFAULTMATERIALS_H

#include <stb_image.h>

#include <string>
#include <vector>
#include "Pong/core/texture.h"
#include "Pong/core/shader.h"
#include "Pong/core/material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pong {
/**Overrides update_shader from material class*/
    class CubeMapMaterial : public Material {
    public:
        CubeMapMaterial(Shader *shader,
                        const TextureUniformVector &textures) :
                Material(shader, textures) {}

        void update_shader(Render *render, Map *scene) override {
            super::update_shader(render, scene);
            // Do not use ubo view for cube maps, because a cube map only should rotate.
            _shader->set_mat4("view", glm::mat4(glm::mat3(
                    scene->get_camera()->get_view_matrix())));
        }
    };

    class CubeMapTexture : public Texture {
        std::vector<std::string> _cubemap_textures;

    public:
        CubeMapTexture(const std::string &right, const std::string &left,
                       const std::string &top, const std::string &bottom,
                       const std::string &front, const std::string &back) {
            _cubemap_textures.push_back(right);
            _cubemap_textures.push_back(left);
            _cubemap_textures.push_back(top);
            _cubemap_textures.push_back(bottom);
            _cubemap_textures.push_back(front);
            _cubemap_textures.push_back(back);

            _texture_id = load_cubemap_textures(_cubemap_textures);
            _gl_bind_type = GL_TEXTURE_CUBE_MAP;
        }

        _P_STATIC _P_INLINE uint32_t load_cubemap_textures(const std::vector<std::string> &faces) {
            uint32_t texture_id;

            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

            int width, height, nrChannels;
            for (uint32_t i = 0; i < faces.size(); ++i) {
                uint8_t *data = stbi_load(faces[i].c_str(),
                                          &width,
                                          &height,
                                          &nrChannels, 0);
                if (data) {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                                 width, height,
                                 0, GL_RGB, GL_UNSIGNED_BYTE, data);

                    stbi_image_free(data);
                } else {
                    LOG_ERROR("Skybox texture failed to load at path: " << faces[i]);
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
    };
}

#endif //PONG_DEFAULTMATERIALS_H
