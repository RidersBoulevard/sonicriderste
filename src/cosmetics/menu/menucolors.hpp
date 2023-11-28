#pragma once

#include "containers/graphicalobject.hpp"

struct MainMenuObject1 {
	fillerData<0xE> filler;
    u8 colorPreset;
};

ASMUsed void RandomizeCustomMenuColors(GraphicalObject *graphicalObject, MainMenuObject1 *object);