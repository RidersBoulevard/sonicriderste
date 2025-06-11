#pragma once

#include "riders/player.hpp"
#include "containers/particleparam.hpp"
#include "riders/object.hpp"

struct SuperAuraDetail {
    ParticleParam* particles;
    void** texList;
};

ASMUsed void SuperSonicAuraCXX(const Player &player);
SuperAuraDetail GetSuperAuraDetail(const Player &player);
ASMUsed void UpdateAuraMatrices(Player &player);