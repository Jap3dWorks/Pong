//
// Created by Jordi on 4/23/2022.
//

#ifndef PONG_DEFAULTMATERIALS_H
#define PONG_DEFAULTMATERIALS_H

#include <string>
#include <vector>
#include "Pong/Core/material.h"
#include "Pong/Core/shader.h"
#include "Pong/Core/texture.h"
#include "Pong/Core/render.h"
#include "Pong/Core/scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

namespace Pong {
/**Overrides update_shader from material class*/
    class SKyBoxMaterial : public Material {
    public:
        SKyBoxMaterial(std::string name, Shader *shader, std::vector<Texture *> textures) :
                Material(std::move(name), shader, std::move(textures)) {}

        void update_shader(Render *render, Scene *scene) override {
            update_params();
            // Do not use ubo view for cube maps, because a cube map only should rotate.
            _shader->setMat4("view", glm::mat4(glm::mat3(
                    scene->get_camera()->get_view_matrix())));
        }
    };

    class SkyBox : public Texture
    {
        std::vector<std::string> _cubemap_textures;

    protected:

    public:
        SkyBox(std::string name, std::string texture_type,
               const std::string &right, const std::string &left, const std::string &top,
               const std::string &bottom, const std::string &front, const std::string &back) :
                Texture(std::move(name), std::move(texture_type))
                {
            _cubemap_textures.push_back(right);
            _cubemap_textures.push_back(left);
            _cubemap_textures.push_back(top);
            _cubemap_textures.push_back(bottom);
            _cubemap_textures.push_back(front);
            _cubemap_textures.push_back(back);

            _texture_id = load_skybox_textures(_cubemap_textures);
            _gl_bind_type = GL_TEXTURE_CUBE_MAP;
        }

        _P_STATIC _P_INLINE unsigned int load_skybox_textures(const std::vector<std::string>& faces) {
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
