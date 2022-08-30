//
// Created by Jordi on 8/5/2022.
//

#ifndef PONG_SRC_PONG_REGISTERS_PARAMETER_MAP_H_
#define PONG_SRC_PONG_REGISTERS_PARAMETER_MAP_H_

#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>
#include <optional>


namespace pong {

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


    class BaseTypeValue {
    public:
        virtual ~BaseTypeValue() = default;
        virtual void *get() noexcept = 0;
    };

    template<typename T>
    class TypeValue : public BaseTypeValue {
    public:
        using InternalType = T;

    private:
        InternalType value_;

    private:
        template<typename ...Args>
        explicit TypeValue(Args ...args) {
            value_ = InternalType(std::forward<Args>(args)...);
        }

    public:
        TypeValue(const TypeValue &) = delete;

        TypeValue(TypeValue &&) = delete;

        TypeValue &operator=(const TypeValue &) = delete;

        TypeValue &operator=(TypeValue &&) = delete;

        ~TypeValue() override = default;

        void *get() noexcept override  {
            return (void *) &value_;
        }

    public:
        friend class ParameterMap;
    };

    /**
     * ParameterMap is en essence an unordered_map<hash, Type()>, so each entry
     * of the class is the hash of the registered class, and an unique
     * instance of the registered class is stored as value.
     * This class is been used for components storage, so each component has its own data structure storage
     * class instance.
     * In the case above ParameterMap do not apply any restriction for parallelism id between data structures,
     * this means that a TransformComponent can share id with a CameraComponent.
     * */
    class ParameterMap {
    public:
        using hash_type = size_t;
        using reg_wrapper = BaseTypeValue;

        using reg_wrapper_ptr = std::unique_ptr<reg_wrapper>;

        template<typename reg_t_>
        using reg_type = TypeValue<reg_t_>;

    private:
        std::unordered_map<hash_type, reg_wrapper_ptr> _data_map;

    private:
        template<typename reg_t_>
        hash_type hash() {
            return typeid(reg_t_).hash_code();
        }

        template<typename reg_t_>
        hash_type hash(const std::string &param_name) {
            if (param_name.empty()) {
                return hash<reg_t_>();
            } else {
                std::string class_name = typeid(reg_t_).name();
                auto hash_name = class_name + "__" + param_name;

                return std::hash<decltype(hash_name)>()(hash_name);
            }
        }

        bool contains_hash(const hash_type& hash_code) const {
            if (_data_map.contains(hash_code)) {
                throw ClassMapException(
                    "Type already registered in ParameterMap."
                );
            }
        }

    public:
        template<typename reg_t_>
        auto &registry(const std::string& param_name="") {
            assert_not_registered<reg_t_>();
            auto hash_code = hash<reg_t_>(param_name);
            auto ptr = new reg_type<reg_t_>();

            _data_map[hash_code] = reg_wrapper_ptr(ptr);
            return *ptr;
        }

        template<typename reg_t_>
        [[nodiscard]] inline bool contains(const std::string &param_name = "") {
            return _data_map.contains(hash<reg_t_>(param_name));
        }

        template<typename reg_t_>
        inline void assert_not_registered(const std::string &param_name = "") {
            auto hash_code = hash<reg_t_>(param_name);
            contains_hash(hash_code);
        }

        template<typename reg_t_>
        inline auto &get(const std::string &param_name="") {
            return *(static_cast<reg_t_ *>(_data_map[hash<reg_t_>(param_name)].get()->get()));
        }
    };
}
#endif //PONG_SRC_PONG_REGISTERS_PARAMETER_MAP_H_
