#ifndef COMPONENT_H
#define COMPONENT_H

#include "Pong/core/actor/actor.h"
#include "Pong/core/collider.h"
#include "Utils/subclasses_map.h"
#include "Utils/fixed_address_buffer.h"
#include "Utils/logger.h"

#include <vector>
#include <cassert>
#include <concepts>
#include <string>
#include <iostream>
#include <algorithm>

namespace Pong {
//    class Actor;

    /**
     *  Component class, an Actor has a list of components, each Component
     *  can contain a custom script you only have to inherit from Component.
     *
    */

    class Component {
    public:
        std::string name;

    public:
        using ComponentsPtrVector = std::vector<Component*>;

    private:
            ComponentsPtrVector _observers;

    protected:
        using component_super = Component;
        bool active = true;

    public:
//        Actor *actor = nullptr;
        Component *parent = nullptr;

    public:
        ~Component()=default;
        explicit Component(std::string name="", Actor *_actor = nullptr, Component *_parent = nullptr) :
            name(std::move(name)), actor(_actor), parent(_parent) {}

        void subscribe(Component *observer) noexcept {
            if (observer->parent) {
                observer->parent->unsubscribe(observer);
            }

            observer->actor = actor;
            observer->parent = this;
            _observers.push_back(observer);
        }

        [[nodiscard]] bool contains(Component *observer) const noexcept {
            return std::find(_observers.begin(),
                             _observers.end(),
                             observer) != _observers.end();
        }

        void unsubscribe(Component *observer) noexcept {
            _observers.erase(
                    std::find(_observers.begin(),
                              _observers.end(),
                              observer));
        }

        virtual void start_wrapper() {
            start();
            for(auto& comp: _observers) {
                comp->start_wrapper();
            }
        }

        virtual inline void start()=0;
        virtual inline void update()=0;

        virtual void emit() {
            for (auto& obsrv: _observers) {
                obsrv->update_wrapper();
            }
        }

        virtual void update_wrapper() {
            if (active) {
                emit();
                update();
            }
        }
    };

    using ComponentFixedBuffer = FixedAddressBuffer<Component>;
    using ComponentMap = SubClassMap<Component>;

}

#endif // COMPONENT_H