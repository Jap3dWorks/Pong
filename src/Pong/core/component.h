#ifndef COMPONENT_H
#define COMPONENT_H

#include "actor.h"
#include "collider.h"
#include "../../utils/subclasses_map.h"
#include "../../utils/fixed_address_buffer.h"

#include "../../utils/logger.h"
#include <vector>
#include <cassert>

#include <iostream>

namespace Pong {
    class Actor;

    /**
     *  Component class, an Actor has a list of components, each Component
     *  can contain a custom script you only have to inherit from Component.
     *
    */
    class Component {
    public:
        using ComponentsPtrVector = std::vector<Component*>;

    private:
//            FixedAddressBuffer<Component> _storage;
            ComponentsPtrVector _observers;

    protected:
        using component_super = Component;
        bool active = True;

    public:
        Actor* actor=nullptr;
        Component* parent=nullptr;

    public:
        ~Component()=default;
        Component(Actor *_actor = nullptr, Component *_parent = nullptr) :
                actor(_actor), parent(_parent) {}

        void subscribe(Component *observer) noexcept {
            if (observer->parent) {
                observer->parent->unsubscribe(observer);
            }

            observer->actor = actor;
            observer->parent = this;
            _observers.push_back(observer);
        }

        [[nodiscard]] bool contains(Component *observer) const noexcept {
            return _observers.find(observer) != _observers.end();
        }

        void unsubscribe(Component *observer) noexcept {
            _observers.erase(_observers.find(observer));
        }

//        template<typename T, typename ...Args>
//        virtual void emplace(Args&& ...args) {
//            auto ptr = _storage.emplace<T>(std::forward<Args>(args)...);
//            _observers.push_back(ptr);
//        }

        virtual void start_wrapper() {
            start(actor, parent);
            for(auto& comp: _observers) {
                comp->start_wrapper(actor, this);
            }
        }

        virtual inline void start()=0;
        virtual inline void update()=0;

        virtual void emit() {
            for (auto& obsrv: _observers) {
                obsrv->update_wrapper()
            }
        }

        virtual void update_wrapper() {
            if (active) {
                emit();
                update(actor, parent);
            }
        }
    };

    using ComponentFixedBuffer = FixedAddressBuffer<Component>;
    using ComponentMap = SubClassMap<Component>;

}

#endif // COMPONENT_H