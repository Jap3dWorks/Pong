//
// Created by Jordi on 8/5/2022.
//

#ifndef GL_TEST_TYPE_REG_H
#define GL_TEST_TYPE_REG_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>


namespace Pong {

    class ClassMapException : std::exception {
    public:
        using message_type = const char *;
    private:
        message_type what_;
    public:
        explicit ClassMapException(message_type message) :
                what_(message) {}

        message_type what() noexcept {
            return what_;
        }
    };


    class TypeReg_ {
    public:
        virtual ~TypeReg_() = default;
        virtual void *get() noexcept = 0;
    };


    template<typename T>
    class TypeReg : public TypeReg_ {
    public:
        using internal_type = T;

    private:
        std::unique_ptr<internal_type> value_;

    private:
        template<typename ...Args>
        explicit TypeReg(Args ...args) {
            value_ = std::make_unique<internal_type>(
                std::forward<Args>(args)...
            );
        }

    public:
        TypeReg(const TypeReg &) = delete;

        TypeReg(TypeReg &&) = delete;

        TypeReg &operator=(const TypeReg &) = delete;

        TypeReg &operator=(TypeReg &&) = delete;

        ~TypeReg() override = default;

        void *get() noexcept override  {
            return (void *) value_.get();
        }

    public:
        friend class TypeRegMap;
    };

    class TypeRegMap {
    public:
        using hash_type = size_t;
        using reg_wrapper = TypeReg_;

        using reg_wrapper_ptr = std::unique_ptr<reg_wrapper>;

        template<typename reg_t_>
        using reg_type = TypeReg<reg_t_>;

    private:
        std::unordered_map<hash_type, reg_wrapper_ptr> _data_map;

    private:
        template<typename reg_t_>
        hash_type hash_() {
            return typeid(reg_t_).hash_code();
        }

    public:
        template<typename reg_t_>
        auto &registry() {
            assert_not_registered<reg_t_>();
            auto hash_code = hash_<reg_t_>();
            auto ptr = new reg_type<reg_t_>();

            _data_map[hash_code] = reg_wrapper_ptr(ptr);
            return *ptr;
        }

        template<typename reg_t_>
        inline bool contains() {
            return _data_map.contains(hash_<reg_t_>());
        }

        template<typename reg_t_>
        inline void assert_not_registered() {
            auto hash_code = hash_<reg_t_>();

            if (_data_map.contains(hash_code)) {
                throw ClassMapException(
                        "Type already registered in TypeRegMap."
                );
            }
        }

        template<typename reg_t_>
        inline auto &get() {
            return *(static_cast<reg_t_ *>(_data_map[hash_<reg_t_>()].get()->get()));
        }
    };
}
#endif //GL_TEST_TYPE_REG_H
