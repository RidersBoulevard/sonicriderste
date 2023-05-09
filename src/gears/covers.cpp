#include "covers.hpp"
#include "lib/sound.hpp"

inline u32 Player_CoverS_ChangeArchetype(Player *player, u32 shortcutType) {
    //u32 archetypeCase;
    if (player->typeAttributes == shortcutType) shortcutType = 2; // same mode toggled, does nothing
    else if (player->rings < 30) shortcutType = 0; // rings less than requirement does nothing
    else shortcutType = 1; // different mode toggled

    return shortcutType;
}

void Player_CreateCoverSParticles(Player *player) {
    auto *particles = (ParticleTaskObject1 *)SetTask(func_Particle_Task, 0xB3B0, 2)->object;
    particles->unk72 = player->index;
    particles->unk0 = 0.0f;
    particles->unk8 = 0.0f;
    particles->unk10 = 0.0f;
    particles->unk18 = 0.0f;
    particles->unk4 = 0.3f;
    particles->unk14 = 0.8f;
    particles->unk68 = gpsTexList_Particle;
    particles->unk60 = &lbl_001D8F58;
    particles->unk73 = -1;
    particles->unk6C = 0;
    particles->unk74 = 0;
    particles->unk48 = &player->x;
}

void Player_CoverS(Player *player) {
    if (player->extremeGear != CoverS) return;
    player->level = 1;
    // player->gearStats[2].airDrain = 20;
    player->gearStats[1].boostCost = player->gearStats[2].boostCost * 0.75f;
    // player->gearStats[2].driftCost = player->gearStats[1].driftCost;
    // player->gearStats[2].tornadoCost = player->gearStats[1].tornadoCost;
    player->gearStats[1].boostSpeed = player->gearStats[2].boostSpeed - pSpeed(10.0f);

    if (player->state == StartLine) player->currentAir = player->gearStats[player->level].maxAir;
    if (player->rings < 20) return;
	u32 shortcutType;
    if (player->input->toggleFaceButtons & DPadLeft) {
        // Speed Type
        shortcutType = 1;
        switch (Player_CoverS_ChangeArchetype(player, shortcutType)) {
            default:
                return;
            case 1:
                player->rings -= 30;
                player->typeAttributes = SpeedType;
                break;
            case 2:
                return;
        }
        Player_CreateCoverSParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
    } else if (player->input->toggleFaceButtons & DPadUp) {
        // Fly Type
        shortcutType = 2;
        switch (Player_CoverS_ChangeArchetype(player, shortcutType)) {
            default:
                return;
            case 1:
                player->rings -= 30;
                player->typeAttributes = FlyType;
                break;
            case 2:
                return;
        }
        Player_CreateCoverSParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
    } else if (player->input->toggleFaceButtons & DPadRight) {
        // Power Type
        shortcutType = 4;
        switch (Player_CoverS_ChangeArchetype(player, shortcutType)) {
            default:
                return;
            case 1:
                player->rings -= 30;
                player->typeAttributes = PowerType;
                break;
            case 2:
                return;
        }
        Player_CreateCoverSParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
    }
}