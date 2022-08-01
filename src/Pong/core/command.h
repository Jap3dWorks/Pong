//
// Created by Jordi on 7/27/2022.
//

#ifndef GL_TEST_COMMAND_H
#define GL_TEST_COMMAND_H

#include <string>
#include <deque>
#include <memory>
#include <unordered_map>


namespace Pong {
    class PCommand;
}

namespace Pong {
    namespace {
        class PCommandFactory_ {
        public:
            virtual std::unique_ptr<PCommand> create() = 0;
        };
    }

    template<typename T>
    class PCommandFactory: public PCommandFactory_ {

    public:
        std::unique_ptr<PCommand> create() override {
            return std::make_unique<T>();
        }
    };

    class PCommandReg {
    public:
        using FactoryPtr = std::unique_ptr<PCommandFactory_>>;
    private:
        std::unordered_map<std::string, FactoryPtr> command_register;

        template<typename T>
        void register_command() {
            command_register[T::name] = std::unique_ptr<PCommandFactory_>(new PCommandFactory<T>);
        }

        std::unique_ptr<PCommand> create_command(const std::string& command_name) {
            return command_register[command_name]->create();
        }

    };

    class PCommand {
    public:
        using c_str = const char*;
    public:
        inline static c_str name="PCommand";

    public:
//        virtual c_str get_name() {
//            return d;
//        }

        virtual void doit()=0;
        virtual void undo(){}
    };

    class PCommandQueue {
    public:
        volatile std::deque<std::unique_ptr<PCommand>> command_deque;
    };
}
#endif //GL_TEST_COMMAND_H
