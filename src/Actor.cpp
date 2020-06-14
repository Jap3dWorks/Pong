#include "Actor.h"
#include "logger.h"

#include <algorithm>

namespace Pong {
    Actor::~Actor() {
        // remove components
        for (auto const &c : _componentList)
            delete c;
        LOG_DEBUG("End Actor destructor")
    }

    void Actor::setScale(const glm::vec3 &scale) {
        _transform = glm::scale(_transform, scale);
    }

    void Actor::draw() const {
        // draw if is visible and has a shape
        if (_visible && _shape) {
            _material->set_param("model", _transform);
            _material->use();

            _shape->draw();

            _material->end_use();
        }
    }

    void Actor::ProcessKeyboard(Movements movement, float deltaTime) {}

    void Actor::setCollider(Collider *coll) {
        _collider = coll;
        coll->actor = this;
    }

    template<typename T>
    void Actor::addComponent(T *c_ptr) {
        if (!std::is_base_of<Component, T>::value)
            return;

        // create new component if not has input
        if (!c_ptr)
            c_ptr = new T;

        // A derivate component class
        c_ptr->addActor(this);
        _componentList.push_back(static_cast<Component *>(c_ptr));
    }

    // --AKinetic--
    // ------------
    AKinetic::AKinetic(std::string name, glm::vec3 vector_director) :
            Actor(name), _vector_director(vector_director) {
        direction = glm::normalize(vector_director);
        _velocity = glm::length(vector_director);
    }

    AKinetic::~AKinetic() noexcept {LOG_DEBUG("AKinetic destructor");}

    void AKinetic::update(float delta_time)
    {
        _vector_director = direction * _velocity;
        _transform = glm::translate(_transform, _vector_director * delta_time);
    }

    // getters
    glm::vec3 AKinetic::getDirection() const {
        return direction;
    }

    float AKinetic::getVelocity() const {
        return _velocity;
    }

    // setters
    void AKinetic::set_direction(glm::vec3 direction) {
        // normalized vector, only should represent the direction.
        // velocity
        direction = glm::normalize(direction);
    }

    void AKinetic::setVelocity(float velocity) {
        // velocity would be the magnitude
        _velocity = velocity;
    }

    // --Player--
    // ----------
    void APlayer::ProcessKeyboard(Movements direction, float delta_time) {
        float f_speed = _base_speed * delta_time;

        if (direction == Pong::Movements::UP) {
            _velocity += f_speed;
            _key_pressed = true;
        } else if (direction == Pong::Movements::DOWN) {
            _velocity -= f_speed;
            _key_pressed = true;
        }
    }

    APlayer::~APlayer() noexcept {LOG_DEBUG("APlayer destructor");}

    void APlayer::update(float delta_time) {
        _vector_director = direction * _velocity;
        _transform = glm::translate(_transform, _vector_director);

        if (!_key_pressed) {
            if (_velocity != 0.f)
                _velocity = _velocity / std::abs(_velocity) * std::max<float>(
                        std::abs(_velocity) - (_base_drag * delta_time), 0.f);
        }
        _key_pressed = false;
    }

    const float Camera::YAW = -90.f;
    const float Camera::PITCH = 0.f;
    const float Camera::SPEED = 2.5f;
    const float Camera::SENSITIVITY = .25f;
    const float Camera::ZOOM = 45.0f;

    ABall::~ABall() noexcept {LOG_DEBUG("ABall destructor");}
}