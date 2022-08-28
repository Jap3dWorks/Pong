//
// Created by Jordi on 7/27/2022.
//

#ifndef PONG_SRC_PONG_CORE_COMMAND_H_
#define PONG_SRC_PONG_CORE_COMMAND_H_

#include "Utils/hash_utils.h"
#include "Utils/reflection_utils.h"
#include "Utils/thread_safe_queue.h"
#include <string>
#include <deque>
#include <memory>
#include <unordered_map>
#include <optional>
#include <cassert>
#include <concepts>


namespace pong {

    template<typename T, typename U>
    struct valid_names {
        static inline constexpr bool valid() {
            if constexpr(T::hash_name() != U::hash_name()) {
                return true;
            }
            return false;
        }
    };

    template<typename Derived_, typename Base_> concept ValidCommand =
        std::derived_from<Derived_, Base_> && valid_names<Derived_, Base_>::valid();


#define ACCESS_HASH_NAME_(class_)                                           \
    template<typename T, typename U>                                        \
    friend struct pong::valid_names;


#define COMMAND_NAME_(command_name_)                                        \
    public:                                                                 \
    using c_str = const char *;                                             \
    inline static constexpr c_str name{command_name_};                      \
    private:                                                                \
    static inline constexpr size_t hash_name() {                            \
        return fnv1a_32(command_name_, sizeof(command_name_) - 1);  \
        }


#define COMMAND_SETUP(command_name_, class_)                                \
    COMMAND_NAME_(command_name_)                                            \
    ACCESS_HASH_NAME_(class_)


    class Command {
    COMMAND_SETUP("Command", Command);

    protected:
        using super = Command;

    public:
        virtual void doit() = 0;
        virtual void undo() {}
    };


    namespace {
        class CommandFactory_ {
        public:
            virtual std::unique_ptr<Command> create() = 0;
        };
    }


    template<ValidCommand<Command> T>
    class CommandFactory : public CommandFactory_ {

    public:
        std::unique_ptr<Command> create() override {
            return std::make_unique<T>();
        }
    };


    class CommandReg {
    public:
        using FactoryPtr = std::unique_ptr<CommandFactory_>;

    private:
        std::unordered_map<std::string, FactoryPtr> command_register;

    public:
        template<ValidCommand<Command> T>
        void register_command() {
            command_register[T::name] = std::unique_ptr<CommandFactory_>(new CommandFactory<T>);
        }

        std::unique_ptr<Command> create_command(const std::string &command_name) {
            return command_register[command_name]->create();
        }

    };


    using CommandQueue = ThreadsafeQueue<std::unique_ptr<Command>>;


    class CommandParser {
    };


}
#endif //PONG_SRC_PONG_CORE_COMMAND_H_
