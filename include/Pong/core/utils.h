#ifndef UTILS_H
#define UTILS_H

#include <yvals_core.h>
#include "Pong/core/core_vals.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <chrono>


_P_INLINE void cout_matrix(_P_CONST glm::mat4& matrix) {
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

template<typename _Base, typename _Derived>
_P_INLINE void assert_base_class() {
#ifdef NDEBUG
#else
    auto is_base = std::is_base_of_v<_Base, _Derived>;
    assert(is_base);
#endif
}

#endif // !UTILS_H

