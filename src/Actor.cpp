#include "Actor.h"
#include "logger.h"

#include <algorithm>

namespace Pong {
    Actor::~Actor() {
        // remove components
        for (auto const &c : _componentList)
            delete c;
    }

    void Actor::set_scale(const glm::vec3 &scale) {
        _transform = glm::scale(_transform, scale);
    }

    void Actor::draw() const {
        // draw if is visible and has a shape
        if(! _visible) return;

        for (unsigned int i=0; i <  _shapes.size(); ++i)
        {
            _materials[i]->use();
            _shapes[i]->draw();
            _materials[i]->end_use();
        }
    }

    void Actor::process_keyboard(Movements movement, float deltaTime) {}

    void Actor::add_collider(Collider* coll)
    {
        _colliders.push_back(coll);
        coll->actor = this;
    }

    template<typename T>
    void Actor::add_component(T * component) {
        if (!std::is_base_of<Component, T>::value)
            return;

        // create new component if not has input
        if (!component)
            component = new T;

        // A derivate component class
        component->addActor(this);
        _componentList.push_back(static_cast<Component *>(component));
    }

    // --AKinetic--
    // ------------
    AKinetic::AKinetic(std::string name, glm::vec3 vector_director) :
            Actor(name), _vector_director(vector_director) {
        _direction = glm::normalize(vector_director);
        _velocity = glm::length(vector_director);
    }

    AKinetic::~AKinetic() noexcept {LOG_DEBUG("AKinetic destructor");}

    void AKinetic::update(float delta_time)
    {
        _vector_director = _direction * _velocity;
        _transform = glm::translate(_transform, _vector_director * delta_time);
    }

    // getters
    glm::vec3 AKinetic::getDirection() const {
        return _direction;
    }

    float AKinetic::getVelocity() const {
        return _velocity;
    }

    // setters
    void AKinetic::set_direction(const glm::vec3& input_direction) {
        this->_direction = glm::normalize(input_direction);
    }

    void AKinetic::setVelocity(float input_velocity) {
        // input_velocity would be the magnitude
        _velocity = input_velocity;
    }

    // --Player--
    // ----------
    void APlayer::process_keyboard(Movements move_direction, float delta_time) {
        float f_speed = _base_speed * delta_time;

        if (move_direction == Pong::Movements::UP) {
            _velocity += f_speed;
            _key_pressed = true;
        } else if (move_direction == Pong::Movements::DOWN) {
            _velocity -= f_speed;
            _key_pressed = true;
        }
    }

    APlayer::~APlayer() noexcept {LOG_DEBUG("APlayer destructor");}

    void APlayer::update(float delta_time) {
        _vector_director = _direction * _velocity;
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