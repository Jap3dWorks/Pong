#include "Actor.h"
#include "../logger.h"
#include "Component.h"

#include <algorithm>
#include <utility>
#include "../Graphic/Material.h"

namespace Pong {
    Actor::~Actor() {
        // remove components
        for (auto const &c : _components)
            delete c;
    }

    void Actor::set_scale(const glm::vec3 &scale) {
        _transform = glm::scale(_transform, scale);
    }

    void Actor::draw(const Render *render, const Scene *scene, Material *material) const {
        // draw if is visible and has a shape
        material->set_param("model", _transform);

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
        _components.push_back(static_cast<Component *>(component));
    }

    // ASkyBox
    // -------
    void ASkyBox::draw(const Render *render, const Scene *scene, Material *material) const
    {
        material->set_param("model", _transform);
    }

    // --AKinetic--
    // ------------
    AKinetic::AKinetic(std::string name, glm::vec3 vector_director) :
            Actor(std::move(name)), _vector_director(vector_director) {
        _direction = glm::normalize(vector_director);
        _velocity = glm::length(vector_director);
    }

    AKinetic::~AKinetic() noexcept {}

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

    APlayer::~APlayer() noexcept {}

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

    const float ACamera::YAW = -90.f;
    const float ACamera::PITCH = 0.f;
    const float ACamera::SPEED = 2.5f;
    const float ACamera::SENSITIVITY = .25f;
    const float ACamera::ZOOM = 45.0f;

    glm::mat4 ACamera::get_view_matrix() const {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ACamera::process_keyboard(Pong::Movements direction, float delta_time) {
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

    ABall::~ABall() noexcept {}
}