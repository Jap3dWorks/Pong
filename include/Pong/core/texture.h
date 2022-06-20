//
// Created by Jordi on 4/19/2022.
//

#ifndef PONG_TEXTURE_H
#define PONG_TEXTURE_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>
#include <utility>
#include <vector>

#include "Pong/core/core_vals.h"
#include "Pong/core/actor.h"
#include "Pong/core/scene.h"
#include "Pong/core/graphic_shape.h"
#include "Pong/core/shader.h"
#include "Pong/core/material.h"

namespace Pong {

    enum class TextureWrap : int32_t {
        REPEAT = GL_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT
    };

    enum class TextureMinFilter : int32_t {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    };

    enum class TextureMagFilter : int32_t {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR
    };

    enum class TextureFormat : int32_t {
        RED = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        BGR = GL_BGR,
        RGBA = GL_RGBA,
        BGRA = GL_BGRA,
        RED_INTEGER = GL_RED_INTEGER,
        RG_INTEGER = GL_RG_INTEGER,
        RGB_INTEGER = GL_RGB_INTEGER,
        BGR_INTEGER = GL_BGR_INTEGER,
        RGBA_INTEGER = GL_RGBA_INTEGER,
        BGRA_INTEGER = GL_BGRA_INTEGER,
        STENCIL_INDEX = GL_STENCIL_INDEX,
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL
    };

    class Texture {
    protected:
        std::string _name;
        std::string _path;
        uint32_t _texture_id{};
        int32_t _gl_bind_type{GL_TEXTURE_2D};
        bool _gamma_corrected{false};

        _P_INLINE uint32_t _load_texture(
                const char *path,
                const bool &gammaCorrection) const {

            uint32_t textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            uint8_t *data = stbi_load(
                    path, &width,
                    &height, &nrComponents, 0);
            if (data) {
                int32_t internalFormat;
                GLenum dataFormat = GL_RED;
                if (nrComponents == 1) dataFormat = internalFormat = GL_RED;
                else if (nrComponents == 3) {
                    internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                    dataFormat = GL_RGB;
                } else if (nrComponents == 4) {
                    internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                    dataFormat = GL_RGBA;
                }

                glBindTexture(_gl_bind_type,
                              textureID);

                glTexImage2D(_gl_bind_type,
                             0,
                             internalFormat,
                             width,
                             height,
                             0,
                             dataFormat,
                             GL_UNSIGNED_BYTE,
                             data);
                glGenerateMipmap(_gl_bind_type);

                glTexParameteri(_gl_bind_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(_gl_bind_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(_gl_bind_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(_gl_bind_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            } else {
                LOG_WARNING("Texture failed to load at path: " << path);
                stbi_image_free(data);
            }

            return textureID;
        }

    public:
        _P_EXPLICIT Texture(const std::string &name) :
                _name(name) {
        }

        Texture(const std::string &name,
                const std::string &path) :
                _name(name),
                _path(path) {
            _texture_id = _load_texture(path.c_str(),
                                        _gamma_corrected);
        }

        virtual ~Texture() = default;

        _P_NODISCARD uint32_t get_id() const {
            return _texture_id;
        }

        _P_NODISCARD uint32_t get_gl_bind_type() const { return _gl_bind_type; }

        std::string get_path() { return _path; }

        std::string get_name() { return _name; }

        void set_wrap(TextureWrap texture_wrap_s, TextureWrap texture_wrap_t) const {
            glBindTexture(GL_TEXTURE_2D, get_id());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            static_cast<
                                    std::underlying_type<TextureWrap>::type
                                    >(texture_wrap_s));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            static_cast<
                                    std::underlying_type<TextureWrap>::type
                                    >(texture_wrap_t));
        }

        void set_min_filter(TextureMinFilter min_filter) const {
            glBindTexture(GL_TEXTURE_2D, get_id());
            glTexParameteri(GL_TEXTURE_2D,
                            GL_TEXTURE_MIN_FILTER,
                            static_cast<
                                    std::underlying_type<TextureMinFilter>::type
                                    >(min_filter));
        }

        void set_mag_filter(TextureMagFilter mag_filter) const {
            glBindTexture(GL_TEXTURE_2D, get_id());
            glTexParameteri(GL_TEXTURE_2D,
                            GL_TEXTURE_MAG_FILTER,
                            static_cast<
                                    std::underlying_type<TextureMagFilter>::type
                                    >(mag_filter));
        }

    };
}


#endif //PONG_TEXTURE_H
