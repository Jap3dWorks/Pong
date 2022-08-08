//
// Created by Jordi on 7/4/2022.
//

#ifndef GL_TEST_CAMERA_COMPONENT_H
#define GL_TEST_CAMERA_COMPONENT_H

#include "Pong/core/core_vals.h"
#include "component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


// TODO: make camera maths using actor translation.
namespace Pong {

    class CameraComponent : public Component {
    public:
        glm::vec3 world_up{0.0, 1.0, 0.0};
        float fov{_P_PI/3.5};

        /**
        Constructor using glm::vec3
         */
    public:
        explicit CameraComponent(
                glm::vec3 _up = glm::vec3(0.f, 1.f, 0.f)
        ) :
            world_up(_up) {}

        void start() override {}
        void update() override {}
        void end() {}

        // vew matrix calculated using euler angles and lookat matrix
//        [[nodiscard]] static glm::mat4 get_view_matrix() {
//            return actor->translation;
//            return glm::lookAt(position, position + front, up);
//        }

        // processes input received from a mouse input system.
//        void process_mouse_movement(float x_offset,
//                                    float y_offset,
//                                    bool constraint_pitch = true) {
//
//            x_offset *= mouse_sensitivity;
//            y_offset *= mouse_sensitivity;
//
//            Yaw += x_offset;
//            Pitch += y_offset;
//
//            // evite screen flipp
//            if (constraint_pitch) {
//                if (Pitch > 89.9f)
//                    Pitch = 89.9f;
//                else if (Pitch < -89.9f)
//                    Pitch = -89.9f;
//            }
//            // update front, right and up vectos
//            update_camera_vectors();
//        }

        // Processes input received from a mouse scroll-wheel event.
//        void process_mouse_scroll(float yoffset) {
//            if (Zoom >= 1.0f && Zoom <= 45.0f)
//                Zoom -= yoffset;
//            else if (Zoom < 1.0f)
//                Zoom = 1.0f;
//            else if (Zoom > 45.0f)
//                Zoom = 45.0f;
//        }

//    private:
//        // calculates the front vector from camera_ptr's euler angles
//        inline void update_camera_vectors() {
//            // calculate new front vector
//            glm::vec3 _front;
//            _front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//            _front.y = sin(glm::radians(Pitch));
//            _front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//
//            front = glm::normalize(front);
//            // also recalculate right and up vector
//            right = glm::normalize(glm::cross(front, world_up));
//            up = glm::normalize(glm::cross(right, front));
//        }
    };
}

#endif //GL_TEST_CAMERA_COMPONENT_H
