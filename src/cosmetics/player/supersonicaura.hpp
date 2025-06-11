#pragma once

#include "riders/player.hpp"
#include "containers/particle_details.hpp"
#include "riders/object.hpp"

struct SuperAuraDetail {
    ParticleDetails* particles;
    void** texList;
};

ASMUsed void SuperSonicAuraCXX(const Player &player);
SuperAuraDetail GetSuperAuraDetail(const Player &player);
ASMUsed void UpdateAuraMatrices(Player &player);