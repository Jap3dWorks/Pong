//
// Created by Jordi on 7/25/2022.
//

#ifndef GL_TEST_EXCEPTIONS_H
#define GL_TEST_EXCEPTIONS_H

#include<exception>

namespace Pong {

    class CreateOutputWindowError : public std::exception {
    private:
        const char *message;
    public:
        using std::exception::exception;

        explicit CreateOutputWindowError(const char *msg) : message(msg) {}

        const char *what() {
            return message;
        }
    };

    class InitializeGladError : public std::exception {
    private:
        const char *message;
    public:
        using std::exception::exception;

        explicit InitializeGladError(const char *msg) : message(msg) {}

        const char *what() {
            return message;
        }
    };



}

#endif //GL_TEST_EXCEPTIONS_H
