#include "dreamtrail.hpp"
#include "riders/object.hpp"
#include "lib/lib.hpp"
#include "containers/particle_details.hpp"

ASMDefined void func_Particle_Task();
ASMDefined ParticleDetails lbl_001FD468;
ASMDefined ParticleDetails lbl_001FD510;

void Player_DreamTrailParticles(Player &player) {
    auto *starParticles = static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
    starParticles->unk72 = player.index;
    starParticles->x = 0.0f;
    starParticles->y = 0.0f;
    starParticles->z = 0.0f;
    starParticles->unk30 = 0.0f;
    starParticles->unk34 = 1.0f;
    starParticles->unk38 = 0.0f;
    starParticles->unk3C = 0.5f;
    starParticles->unk40 = 0.1f;
    starParticles->unk44 = 0.1f;
    starParticles->unk10 = 0.0f;
    starParticles->unk14 = 0.1f;
    starParticles->unk18 = 0.0f;
    starParticles->unk48 = &gcosNnSystemVecZeroFast;
    starParticles->particleParams = &lbl_001FD468;
    starParticles->texList = gpsTexList_Particle;
    starParticles->baseModelMatrix = &player.unkC4;
    starParticles->unk73 = static_cast<u8>(-1);
    starParticles->unk74 = 0;

    auto *lineParticles = static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
    lineParticles->unk72 = player.index;
    lineParticles->x = 0.0f;
    lineParticles->y = 0.0f;
    lineParticles->z = 0.0f;
    lineParticles->unk30 = 0.0f;
    lineParticles->unk34 = 1.0f;
    lineParticles->unk38 = 0.0f;
    lineParticles->unk3C = 0.3f;
    lineParticles->unk40 = -0.6f;
    lineParticles->unk44 = 0.3f;
    lineParticles->unk10 = 0.0f;
    lineParticles->unk14 = 0.3f;
    lineParticles->unk18 = -0.6f;
    lineParticles->unk48 = &gcosNnSystemVecZeroFast;
    lineParticles->particleParams = &lbl_001FD510;
    lineParticles->texList = gpsTexList_Particle;
    lineParticles->baseModelMatrix = &player.unkC4;
    lineParticles->unk73 = static_cast<u8>(-1);
    lineParticles->unk74 = 0;
}