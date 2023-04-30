//
// Created by Jordi on 8/8/2022.
//

#ifndef GL_TEST_MATH_UTILS_H
#define GL_TEST_MATH_UTILS_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace pong::math {

    bool decompose_transforms(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
        using namespace glm;
        using T = float;

        mat4 LocalMatrix(transform);

        // Normalize the matrix.
        if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
            return false;

        // First, isolate perspective.  This is the messiest.
        if (
                epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
                epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
                epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
        {
            // Clear the perspective partition
            LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
            LocalMatrix[3][3] = static_cast<T>(1);
        }

        // Next take care of translation (easy).
        translation = vec3(LocalMatrix[3]);
        LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

        vec3 Row[3];//, Pdum3;

        // Now get scale and shear.
        for (length_t i = 0; i < 3; ++i)
            for (length_t j = 0; j < 3; ++j)
                Row[i][j] = LocalMatrix[i][j];

        // Compute X scale factor and normalize first row.
        scale.x = length(Row[0]);
        Row[0] = detail::scale(Row[0], static_cast<T>(1));
        scale.y = length(Row[1]);
        Row[1] = detail::scale(Row[1], static_cast<T>(1));
        scale.z = length(Row[2]);
        Row[2] = detail::scale(Row[2], static_cast<T>(1));

        // At this point, the matrix (in rows[]) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.
#if 0
        Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
	if (dot(Row[0], Pdum3) < 0)
	{
	    for (length_t i = 0; i < 3; i++)
	    {
		scale[i] *= static_cast<N>(-1);
		Row[i] *= static_cast<N>(-1);
	    }
	}
#endif

        rotation.y = asin(-Row[0][2]);
        if (cos(rotation.y) != 0.f) {
            rotation.x = atan2(Row[1][2], Row[2][2]);
            rotation.z = atan2(Row[0][1], Row[0][0]);
        }
        else {
            rotation.x = atan2(-Row[2][0], Row[1][1]);
            rotation.z = 0;
        }

        return true;
    }

    // vew matrix calculated using euler angles and lookat matrix
        [[nodiscard]] static glm::mat4 get_view_matrix() {
//            return entity->translation;
//            return glm::lookAt(position, position + front, up);

            return {};
    }

        // calculates the front vector from camera_ptr's euler angles
        inline glm::mat3 camera_vectors(const float& Yaw, const float& Pitch, const glm::vec3& world_up) {
            // calculate new front vector
            glm::vec3 front{
                    glm::cos(Yaw) * glm::cos(Pitch),
                    glm::sin(Pitch),
                    glm::sin(Yaw) * glm::cos(Pitch)
            };

            front = glm::normalize(front);
            auto x_vec = glm::normalize(glm::cross(front, world_up));
            auto y_vec = glm::normalize(glm::cross(x_vec, front));

            return {x_vec,
                    y_vec,
                    front};
        }

}



#endif //GL_TEST_MATH_UTILS_H
