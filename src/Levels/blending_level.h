//
// Created by Jordi on 8/23/2020.
//
#ifndef PONG_BLENDING_LEVEL_H
#define PONG_BLENDING_LEVEL_H

#include "../core/Level.h"


class BlendingLevel : public Pong::AbstractLevel {
protected:
    void _level_setup() override;

    inline void _create_blending_actors();

};


#endif //PONG_BLENDING_LEVEL_H
