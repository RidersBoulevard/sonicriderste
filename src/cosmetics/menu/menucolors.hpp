#pragma once

#include "context.hpp"
#include "containers/graphicalobject.hpp"

struct MainMenuObject1 {
    char filler[0xE];
    u8 colorPreset;
};

ASMUsed void RandomizeCustomMenuColors(GraphicalObject *graphicalObject, MainMenuObject1 *object);