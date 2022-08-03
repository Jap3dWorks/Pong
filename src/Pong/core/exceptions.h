//
// Created by Jordi on 7/25/2022.
//

#ifndef GL_TEST_EXCEPTIONS_H
#define GL_TEST_EXCEPTIONS_H

#include<exception>

namespace Pong {

    class CreateOutputWindowError : public std::exception {
    private:
        const char *what_;
    public:
        using std::exception::exception;

        explicit CreateOutputWindowError(const char *msg) : what_(msg) {}

        [[nodiscard]] const char *what() const noexcept override {
            return what_;
        }
    };

    class InitializeGladError : public std::exception {
    private:
        const char *what_;
    public:
        using std::exception::exception;

        explicit InitializeGladError(const char *msg) : what_(msg) {}

        [[nodiscard]] const char *what() const noexcept override {
            return what_;
        }
    };

    // Exceptions
    class MeshException : public std::exception {
    private:
        const char *what_;
    public:
        using std::exception::exception;

        explicit MeshException(const char* msg) :
                what_(msg){}

        [[nodiscard]] const char *what() const noexcept override {
            return what_;
        }
    };

}

#endif //GL_TEST_EXCEPTIONS_H
