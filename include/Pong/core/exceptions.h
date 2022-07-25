//
// Created by Jordi on 7/25/2022.
//

#ifndef GL_TEST_EXCEPTIONS_H
#define GL_TEST_EXCEPTIONS_H

#include<exception>
namespace Pong {
class CreateOutputWindowError: public std::exception {
public:
    using std::exception::exception;
};

class InitializeGladError: public std::exception {
public:
    using std::exception::exception;
};



}

#endif //GL_TEST_EXCEPTIONS_H
