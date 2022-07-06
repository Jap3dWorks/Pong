#ifndef ACTOR_H
#define ACTOR_H

namespace Pong
{
    class Component;
    class Scene;
    class Render;
    class Material;
}

#include "Pong/core/movements.h"
#include "Pong/core/collider.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Pong/core/component.h"

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <list>

namespace Pong {
    // TODO: colliders with transform matrix to offset.
    class Actor {
    public:
        std::string name;
        glm::mat4 transform = glm::mat4(1);
        bool active = true;
        ComponentMap component_map;

    public:
        explicit Actor(std::string _name): name(std::move(_name)) {}

        virtual ~Actor() = default;

//        virtual void process_keyboard(Movements movement, const double& delta_time);

//        virtual void draw(const Render *render, const Scene *scene, Material *material) const;
        virtual void start() {}
        virtual void by_frame(){}

    };

    // sky box
    // -------
    class ASkyBox: public Actor
    {
    public:
        explicit ASkyBox(std::string name): Actor(std::move(name)) {}
        void draw(const Render *render, const Scene *scene, Material *material) const override;
    };


    // TODO: Deprecate below
    // --AKinetic class--
    // ------------------
    class AKinetic : public Actor
    {
    protected:
        glm::vec3 _vector_director{};  // _direction * velocity
        glm::vec3 _direction{};
        float _velocity = 0;
        float _acceleration = 0;

        float _base_drag = 0.f;
        float _base_speed = 0.f;

    public:
        explicit AKinetic(std::string name): Actor(std::move(name)){}
        AKinetic(std::string name, glm::vec3 vector_director);

        ~AKinetic() override;

        void by_frame() override;

        // returns AKinetic vector director
        [[nodiscard]] glm::vec3 getDirection() const;
        [[nodiscard]] glm::vec3 get_vector_director() const { return _vector_director; }

        [[nodiscard]] float getVelocity() const;

        // Set AKinetic vector director
        // normalized vector, only should represent the _direction.
        void set_direction(const glm::vec3& input_direction);


        // input_velocity would be the vector magnitude.
        void setVelocity(float input_velocity);

    };

    // --APLAYER class--
    class APlayer : public AKinetic
    {
    private:
        bool _key_pressed = false;

    public:
        explicit APlayer(std::string name):AKinetic(std::move(name))
        {
            _base_drag = 0.5f;
            _base_speed = 0.1f;
            _direction = glm::vec3{0.f, 1.f, 0.f };
        }
        ~APlayer() override;

        void by_frame(float delta_time) override;

        void process_keyboard(Movements move_direction, const double& delta_time) override;
    };

    // --ABALL class--
    class ABall : public AKinetic
    {
    private:

    public:
        explicit ABall(std::string name, glm::vec3 vector_director=glm::vec3(2,0.f,0.f),
            glm::vec3 start_position=glm::vec3()) :
            AKinetic(std::move(name), vector_director){}

        ~ABall() override;
    };

    // --ACamera--
    // ----------
    /**
    ACamera Actor type preconfigured with input keyboard
     */
     // camera as component
     // input as component
    class ACamera : public Actor {
    public:
        static const float YAW;
        static const float PITCH;
        static const float SPEED;
        static const float SENSITIVITY;
        static const float ZOOM;

        // camera_ptr attributes
        glm::vec3 Position{};
        glm::vec3 Front;
        glm::vec3 Up{};
        glm::vec3 Right{};
        glm::vec3 WorldUp{};

        // Euler angles
        float Yaw;
        float Pitch;

        // ACamera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        /**
        Constructor using glm::vec3*/
        explicit ACamera(std::string name,
                         glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                         glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
                         float yaw = YAW,
                         float pitch = PITCH) : Front(glm::vec3(0.f, 0.f, -1.f)),
                                                MovementSpeed(SPEED),
                                                MouseSensitivity(SENSITIVITY),
                                                Zoom(ZOOM),
                                                Yaw(yaw),
                                                Pitch(pitch),
                                                Position(position),
                                                WorldUp(up),
                                                Actor(std::move(name)) {
            update_camera_vectors();
        }

        // constructor with scalar values
        ACamera(std::string name,
                float posX, float posY, float posZ,
                float upX, float upY, float upZ,
                float yaw = YAW,
                float pitch = PITCH) : Front(glm::vec3(0.f, 0.f, -1.f)),
                                       MovementSpeed(SPEED),
                                       MouseSensitivity(SENSITIVITY),
                                       Zoom(ZOOM),
                                       Actor(std::move(name)) {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            update_camera_vectors();
        }

        // vew matrix calculated using euler angles and lookat matrix
        [[nodiscard]] glm::mat4 get_view_matrix() const;

        // keyboard input
        void process_keyboard(Pong::Movements direction, const double& delta_time) override;

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
        void update_camera_vectors() {
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

#endif // ACTOR_H
