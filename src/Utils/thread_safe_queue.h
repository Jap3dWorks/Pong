#ifndef PONG_THREAD_SAFE_QUEUE_H
#define PONG_THREAD_SAFE_QUEUE_H

#include <algorithm>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

using namespace std;

class non_empty_queue : public std::exception {
    std::string what_;
public:
    explicit non_empty_queue(std::string msg) { what_ = std::move(msg); }
    [[nodiscard]] const char* what() const noexcept override  { return what_.c_str(); }
};

template<typename T>
class ThreadsafeQueue {
    std::queue<T> queue_;
    mutable std::mutex mutex_;

    [[nodiscard]] bool empty() const {
        return queue_.empty();
    }

public:
    ThreadsafeQueue() = default;
    ThreadsafeQueue(const ThreadsafeQueue<T> &) = delete ;
    ThreadsafeQueue& operator=(const ThreadsafeQueue<T> &) = delete ;

    ThreadsafeQueue(ThreadsafeQueue<T>&& other) noexcept(false) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!empty()) {
            throw non_empty_queue(
                    "Moving into a non-empty queue"s
            );
        }
        queue_ = std::move(other.queue_);
    }

    virtual ~ThreadsafeQueue() noexcept(false) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!empty()) {
            throw non_empty_queue(
                    "Destroying a non-empty queue"s
            );
        }
    }

    [[nodiscard]] unsigned long size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    std::optional<T> pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return {};
        }
        T tmp = queue_.front();
        queue_.pop();
        return tmp;
    }

    void push(const T &item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
    }
};

#endif //PONG_THREAD_SAFE_QUEUE_H
