//
// Created by Jordi on 7/14/2022.
//

#ifndef GL_TEST_ACTION_EVENT_H
#define GL_TEST_ACTION_EVENT_H

#include <vector>

template<typename T>
class Event {
private:
    std::vector<T> subscribers;

public:
    template<typename ...Args>
    void operator() (Args&& ...args) const {
        for (auto& s: subscribers) {
            s(std::forward<Args>(args)...);
        }
    }

    Event<T>& operator+=(T other) {
        subscribers.push_back(other);
        return *this;
    }

    Event<T>& operator-=(T other) {
        auto itr = subscribers.find(other);
        if(itr != subscribers.end()) {
            subscribers.erase(itr);
        }

        return *this;
    }

public:
    void subscribe(T _ptr) {
        subscribers.push_back(_ptr);
    }

};

#endif //GL_TEST_ACTION_EVENT_H
