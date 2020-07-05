#include "Utils.h"
#include "logger.h"

//namespace Pong
//{
//    class Scene;
//    class Shader;
//    class Material;
//    class AKinetic;
//    class IcosphereShape;
//}

#include "Actor.h"
#include "Scene.h"
#include "Shader_s.h"
#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>

// utility to load textures
unsigned int loadTexture(char const *path, const bool &gammaCorrection)
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


void cout_matrix(const glm::mat4 &matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << matrix[i][j] << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

namespace Pong {
    void draw_point(glm::vec3 point)
    {
        Scene* scene = Scene::get_instance();

        // point actor name
        std::string pnt_nme = "point_";
        int number = 0;
        while (true)
        {
            std::string try_nme = pnt_nme + std::to_string(number);
            if (scene->actor_map.find(try_nme) == scene->actor_map.end())
            {
                pnt_nme = try_nme;
                break;
            }
            else
            {
                number++;
            }
        }

        // get shader
        Shader* point_shd = scene->create_shader("point_shd", "unlit_v.glsl", "unlit_f.glsl");

        Material* point_mat = scene->create_material("point_mat", point_shd,
                                                     std::vector<Pong::Texture *>());

        // point actor
        auto* a_point = scene->create_actor<AKinetic>(pnt_nme);
        auto* s_point = scene->create_shape<IcosphereShape>("point_shp");
        a_point->add_shape(s_point);
        a_point->set_transform(glm::translate(glm::mat4(1), point));

        a_point->add_material(point_mat);

        s_point->set_radius(0.05f);

        LOG_DEBUG("Point created-> " << pnt_nme << " at-> " <<
                                     point.x << ", " << point.y << ", " << point.z);
    }
}