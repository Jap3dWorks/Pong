#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

// utility to load Textures
unsigned int loadTexture(char const *path, const bool &gammaCorrection);


inline void cout_matrix(const glm::mat4 &matrix);


template<typename T>
inline void cout_buffer(const std::vector<T> v, int col)
{
    for (int i = 0; i < v.size() / col; i++)
    {

        for (int j = 0; j < col; j++)
        {
            std::cout <<  << std::flush;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//inline void cout_vector(glm::vec3 &vector);

namespace Pong {
    void draw_point(glm::vec3 point);
}
#endif // !UTILS_H

