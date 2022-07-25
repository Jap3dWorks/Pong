//
// Created by Jordi on 4/20/2022.
//

#ifndef PONG_CORE_VALS_H
#define PONG_CORE_VALS_H

#include <yvals_core.h>


#define _P_NODISCARD _NODISCARD
#define _P_INLINE _INLINE_VAR
#define _P_STATIC static
#define _P_CONSTEXPR _CONSTEXPR20
#define _P_CONST const
#define _P_EXPLICIT explicit
#define _P_EXTERN extern

#ifdef _HAS_CXX20
    #include <numbers>
    #define _P_PI std::numbers::pi
#else
    #ifndef _USE_PATH_DEFINES
    #define _USE_PATH_DEFINES
    #endif
    #include <cmath>
    #define _P_PI M_PI
#endif // _HAS_CXX20

#endif //PONG_CORE_VALS_H
