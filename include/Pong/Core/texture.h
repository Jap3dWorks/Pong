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
#include <vector>

#include "Pong/Core/core_vals.h"
#include "Pong/Core/actor.h"
#include "Pong/Core/scene.h"
#include "Pong/Core/shape.h"
#include "Pong/Core/shader.h"
#include "Pong/Core/material.h"

// utility to load Textures
_P_INLINE unsigned int load_texture(_P_CONST char *path, _P_CONST bool &gammaCorrection)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
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


#endif //PONG_TEXTURE_H
