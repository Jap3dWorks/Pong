//
// Created by Jordi on 8/8/2022.
//

#ifndef GL_TEST_MATH_UTILS_H
#define GL_TEST_MATH_UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pong::math {

    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

    template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    inline static T DivideAndRoundUp(T dividend, T divisor)
    {
        return (dividend + divisor - 1) / divisor;
    }


    template<typename T, typename DivisorT, std::enable_if_t<std::is_same_v<T, glm::uvec2> && std::is_integral_v<DivisorT>, bool> = true>
    inline static T DivideAndRoundUp(T dividend, DivisorT divisor)
    {
        return { DivideAndRoundUp(dividend.x, divisor), DivideAndRoundUp(dividend.y, divisor) };
    }
}


#endif //GL_TEST_MATH_UTILS_H
