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
        float YAW;
        float PITCH;
        float SPEED;
        float SENSITIVITY;
        float ZOOM;

        // camera_ptr attributes
        glm::vec3 Position{};
        glm::vec3 Front{};
        glm::vec3 Up{};
        glm::vec3 Right{};
        glm::vec3 WorldUp{};

        // Euler angles
        float Yaw = -90.f;
        float Pitch = 0.f;
        // ACamera options
        float MovementSpeed = 2.5f;
        float MouseSensitivity = .25f;
        float Zoom = 45.0f;

        /**
        Constructor using glm::vec3*/
    public:

        void start(Actor* actor, Component* parent=nullptr) {
            component_super::start(actor, parent);
            //
            update_camera_vectors();
        }

        void by_frame(Actor* actor, Component* parent=nullptr) {
            component_super::by_frame(actor, parent);
        }

        CameraComponent(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                         glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
                        float yaw = -90.f,
                        float pitch = 0.f) : Front(glm::vec3(0.f, 0.f, -1.f)),
                                             Yaw(yaw),
                                             Pitch(pitch),
                                             Position(position),
                                             WorldUp(up),
                                             Component() {
//            update_camera_vectors();
        }

        // vew matrix calculated using euler angles and lookat matrix
        [[nodiscard]] glm::mat4 get_view_matrix() const {
            return glm::lookAt(Position, Position + Front, Up);
        }

        // keyboard input
//        void process_keyboard(Pong::Movements direction, const double &delta_time) override;

        // processes input received from a mouse input system.
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch = true) {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;

            // evite screen flipp
            if (constraintPitch) {
                if (Pitch > 89.9f)
                    Pitch = 89.9f;
                else if (Pitch < -89.9f)
                    Pitch = -89.9f;
            }
            // by_frame Front, Right and Up vectos
            update_camera_vectors();
        }

        // Processes input received from a mouse scroll-wheel event.
        void ProcessMouseScroll(float yoffset) {
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
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

            Front = glm::normalize(front);
            // also recalculate right and up vector
            Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));
        }
    };

}

#endif //GL_TEST_CAMERA_COMPONENT_H
