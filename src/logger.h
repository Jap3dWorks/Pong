//
// Created by Jordi on 6/14/2020.
//
#include <iostream>

#ifndef PONG_LOGGER_H
#define PONG_LOGGER_H

// debug
#ifdef __LOG_DEBUG__
#define LOG_DEBUG(message) \
    std::cout << "DEBUG: " << message << std::endl;
#else
#define LOG_DEBUG(message)
#endif

// info
#ifdef __LOG_INFO__
#define LOG_INFO(message) \
    std::cout << "INFO: " << message << std::endl;
#else
#define LOG_INFO(message)
#endif

// warning
#ifdef __LOG_WARNING__
#define LOG_WARNING(message) \
    std::cout << "WARNING: " << message << std::endl;
#else
#define LOG_WARNING(message)
#endif

// Error
#ifdef __LOG_ERROR__
#define LOG_ERROR(message) \
    std::cout << "ERROR: " << message << std::endl;
#else
#define LOG_ERROR(message)
#endif

#endif //PONG_LOGGER_H
