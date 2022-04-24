//
// Created by Jordi on 4/24/2022.
//

#ifndef PONG_SHADING_LEVEL_H
#define PONG_SHADING_LEVEL_H

#include "Core/core_vals.h"
#include "Core/Level.h"
#include "Core/core_vals.h"
#include "Core/Material.h"
#include "Core/Lights.h"
#include "logger.h"
#include "DefaultMaterials.h"
#include "Levels/blending_level.h"

class ShadingLevel : public Pong::AbstractLevel {
protected:
    void _level_setup() override {
        AbstractLevel::_level_setup();

        // add a camera quad



    }
};

#endif //PONG_SHADING_LEVEL_H
