//
// Created by Jordi on 7/4/2022.
//

#ifndef GL_TEST_CAMERA_COMPONENT_H
#define GL_TEST_CAMERA_COMPONENT_H

#include "Pong/core/component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// TODO: make camera maths using actor transform.
namespace Pong {

    class CameraComponent : public Component {
    public:
        // camera_ptr attributes
        glm::vec3 position{};
        glm::vec3 front{};
        glm::vec3 up{};
        glm::vec3 right{};
        glm::vec3 world_up{};

        // Euler angles
        float Yaw = -90.f;
        float Pitch = 0.f;
        // ACamera options
//        float movement_speed = 2.5f;
        float mouse_sensitivity = .25f;
        float Zoom = 45.0f;

        /**
        Constructor using glm::vec3*/
    public:

        inline void start() override {
            update_camera_vectors();
        }

        inline void update() override {
            update_camera_vectors();
        }

        explicit CameraComponent(
                Actor* actor,
                Component *parent,
                glm::vec3 _position = glm::vec3(0.f, 0.f, 0.f),
                glm::vec3 _up = glm::vec3(0.f, 1.f, 0.f),
                float _yaw = -90.f,
                float _pitch = 0.f
        ) : front(glm::vec3(0.f, 0.f, -1.f)),
            Yaw(_yaw),
            Pitch(_pitch),
            position(_position),
            world_up(_up),
            Component(actor, parent) {}

        // vew matrix calculated using euler angles and lookat matrix
        [[nodiscard]] glm::mat4 get_view_matrix() const {
            return glm::lookAt(position, position + front, up);
        }

        // processes input received from a mouse input system.
        void process_mouse_movement(float x_offset,
                                    float y_offset,
                                    bool constraint_pitch = true) {
            x_offset *= mouse_sensitivity;
            y_offset *= mouse_sensitivity;

            Yaw += x_offset;
            Pitch += y_offset;

            // evite screen flipp
            if (constraint_pitch) {
                if (Pitch > 89.9f)
                    Pitch = 89.9f;
                else if (Pitch < -89.9f)
                    Pitch = -89.9f;
            }
            // update front, right and up vectos
            update_camera_vectors();
        }

        // Processes input received from a mouse scroll-wheel event.
        void process_mouse_scroll(float yoffset) {
            if (Zoom >= 1.0f && Zoom <= 45.0f)
                Zoom -= yoffset;
            else if (Zoom < 1.0f)
                Zoom = 1.0f;
            else if (Zoom > 45.0f)
                Zoom = 45.0f;
        }

    private:
        // calculates the front vector from camera_ptr's euler angles
        inline void update_camera_vectors() {
            // calculate new front vector
            glm::vec3 _front;
            _front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            _front.y = sin(glm::radians(Pitch));
            _front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

            front = glm::normalize(front);
            // also recalculate right and up vector
            right = glm::normalize(glm::cross(front, world_up));
            up = glm::normalize(glm::cross(right, front));
        }
    };
}

#endif //GL_TEST_CAMERA_COMPONENT_H
