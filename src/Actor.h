#ifndef ACTOR_H
#define ACTOR_H

namespace Pong
{
    class Component;
}

#include "pongTypes.h"

#include "Shape.h"
#include "Material.h"
#include "Component.h"
#include "Collider.h"

#include <iostream>
#include <vector>
#include <string>
#include <list>

namespace Pong {

    class Actor
    {
    protected:
        bool _visible=true;
        Shape* _shape = nullptr;
        Material* _material = nullptr;
        Collider* _collider = nullptr;
        glm::mat4 _transform = glm::mat4(1);
        std::string _name;
        std::list<Component*> _componentList; // list is faster than vector

    public:
        Actor(std::string name): _name(name) {}

        virtual ~Actor();

        virtual void ProcessKeyboard(Movements movement, float deltaTime);

        virtual void draw() const;

        virtual void update(float delta_time){}

        // --setters--
        void setShape(Shape* shape) { _shape = shape; }
        void setTransform(glm::mat4 trans) { _transform = trans; }
        void setMaterial(Material* mat) { _material = mat; }
        void setCollider(Collider* coll);
        void setVisibility(bool vis) { _visible = vis; }

        // transforms setters
        void setScale(const glm::vec3 &scale);

        // getters
        Shape* getShape() const { return _shape; }
        glm::mat4 getTransform() const { return _transform; }
        Material* getMaterial() const { return _material; }
        Collider* getCollider() const { return _collider; }
        std::string getName() { return _name; }
        std::list<Component*> get_components() { return _componentList; }
        bool getVisibility() { return _visible; }

        // config members
        template <typename T>
        void addComponent(T* c_ptr = nullptr);
    };

    // --AKinetic class--
    // ------------------
    class AKinetic : public Actor
    {
    protected:
        glm::vec3 _vector_director;  // direction * velocity
        glm::vec3 direction;
        float _velocity = 0;
        float _acceleration = 0;

        float _base_drag = 0.f;
        float _base_speed = 0.f;

    public:
        AKinetic(std::string name): Actor(name){}
        AKinetic(std::string name, glm::vec3 vector_director);

        virtual ~AKinetic() { std::cout << "AKinetic destructor\n"; }

        virtual void update(float delta_time);

        // returns AKinetic vector director
        glm::vec3 getDirection() const;
        glm::vec3 get_vector_director() const { return _vector_director; }

        float getVelocity() const;

        // Set AKinetic vector director
        // normalized vector, only should represent the direction.
        void set_direction(glm::vec3);


        // velocity would be the vector magnitude.
        void setVelocity(float velocity);

    };

    // --APLAYER class--
    class APlayer : public AKinetic
    {
    private:
        bool _key_pressed = false;

    public:
        APlayer(std::string name):AKinetic(name)
        {
            _base_drag = 0.5f;
            _base_speed = 0.1f;
            direction = glm::vec3{ 0.f, 1.f, 0.f };
        }
        virtual ~APlayer() { std::cout << "APlayer destructor\n"; }

        void update(float delta_time) override;

        virtual void ProcessKeyboard(Movements movement, float delta_time) override;
    };

    // --ABALL class--
    class ABall : public AKinetic
    {
    private:

    public:
        ABall(std::string name, glm::vec3 vector_director=glm::vec3(2,0.f,0.f),
            glm::vec3 start_position=glm::vec3()) :
            AKinetic(name, vector_director){}

        virtual ~ABall() { std::cout << "ABall destructor\n"; }
    };

    // --Camera--
    // ----------
    class Camera : public Actor {
    public:
        static const float YAW;
        static const float PITCH;
        static const float SPEED;
        static const float SENSITIVITY;
        static const float ZOOM;

        // camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // Euler angles
        float Yaw;
        float Pitch;

        // Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        // constructor with vectors
        Camera(std::string name,
               glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
               glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
               float yaw = YAW,
               float pitch = PITCH) : Front(glm::vec3(0.f, 0.f, -1.f)),
                                      MovementSpeed(SPEED),
                                      MouseSensitivity(SENSITIVITY),
                                      Zoom(ZOOM),
                                      Actor(name) {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        // constructor with scalar values
        Camera(std::string name,
               float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw = YAW,
               float pitch = PITCH) : Front(glm::vec3(0.f, 0.f, -1.f)),
                                      MovementSpeed(SPEED),
                                      MouseSensitivity(SENSITIVITY),
                                      Zoom(ZOOM),
                                      Actor(name) {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        // vew matrix calculated using euler angles and lookat matrix
        glm::mat4 GetViewMatrix() {
            return glm::lookAt(Position, Position + Front, Up);
        }

        // keyboard input
        virtual void ProcessKeyboard(Pong::Movements direction, float delta_time) {
            float velocity = MovementSpeed * delta_time;
            if (direction == Pong::Movements::FORWARD)
                Position += Front * velocity;
            if (direction == Pong::Movements::BACKWARD)
                Position -= Front * velocity;
            if (direction == Pong::Movements::LEFT)
                Position -= Right * velocity;
            if (direction == Pong::Movements::RIGHT)
                Position += Right * velocity;
            if (direction == Pong::Movements::UP)
                Position += Up * velocity;
            if (direction == Pong::Movements::DOWN)
                Position -= Up * velocity;
        }

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
            // update Front, Right and Up vectos
            updateCameraVectors();
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
        // calculates the front vector from camera's euler angles
        void updateCameraVectors() {
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
