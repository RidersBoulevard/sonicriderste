#include "coverf.hpp"
#include "lib/sound.hpp"


inline u32 Player_CoverF_ChangeArchetype(Player *player, const u8 archetype) {
    u32 archetypeCase;
    if (player->coverF_archetype == archetype && player->rings >= 25) archetypeCase = 2; // same mode toggled
    else if (player->rings < 30) archetypeCase = 0;
    else archetypeCase = 1;

    return archetypeCase;
}

inline void Player_CoverF_UpdateStats(Player *player, const u8 archetype) {
    player->coverF_archetype = archetype;

    player->gearStats[1].topSpeed = CoverF_Speeds[archetype-1][0];
    if (player->state == Cruise) player->speedCap = CoverF_Speeds[archetype-1][0];
    if (player->characterArchetype == BoostArchetype) // boost character support
    {
        player->gearStats[1].boostSpeed = CoverF_SpeedsBoostArchetype[archetype-1][1];
    } else player->gearStats[1].boostSpeed = CoverF_Speeds[archetype-1][1];
}

void Player_CreateCoverFParticles(Player *player) {
    auto *particles = reinterpret_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, 0xB3B0, 2)->object);
    particles->unk72 = player->index;
    particles->unk0 = 0.0f;
    particles->unk8 = 0.0f;
    particles->unk10 = 0.0f;
    particles->unk18 = 0.0f;
    particles->unk4 = 0.3f;
    particles->unk14 = 0.8f;
    particles->unk68 = gpsTexList_Particle;
    particles->unk60 = &lbl_001D8F58;
    particles->unk73 = 0xFF;
    particles->unk6C = 0;
    particles->unk74 = 0;
    particles->unk48 = &player->x;
}

void Player_CoverF(Player *player) {
    u8 archetype;
    s32 airToAdd = player->gearStats[1].maxAir;
    // u32 isCruising = (player->state == Cruise) ? TRUE : FALSE;

    if (player->extremeGear == CoverF && player->state == StartLine)
    {
        player->level = 1;
        player->gearStats[1].boostSpeed = player->gearStats[0].boostSpeed;
        player->currentAir = player->gearStats[player->level].maxAir;
    }
    if (player->extremeGear != CoverF || player->rings < 25) return;



    if (player->input->toggleFaceButtons & DPadLeft) {
        // Solution Turbo (ST)
        archetype = 2;
        switch (Player_CoverF_ChangeArchetype(player, archetype)) {
            default:
                return;
            case 1:
                player->changeInAir_gain += airToAdd;
                player->rings -= 30;
                break;
            case 2:
                airToAdd /= 2;
                player->changeInAir_gain += airToAdd;
                player->rings -= 25;
                break;
        }
        Player_CoverF_UpdateStats(player, archetype);

        player->gearStats[0].boostCost = 45000;
        player->gearStats[1].boostCost = player->gearStats[1].maxAir / 2;
        player->specialFlags &= ~berserkerEffect;

        Player_CreateCoverFParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);


    } else if (player->input->toggleFaceButtons & DPadRight) {
        // Solution Cruise (SC)
        archetype = 1;
        switch (Player_CoverF_ChangeArchetype(player, archetype)) {
            default:
                return;
            case 1:
                player->changeInAir_gain += airToAdd;
                player->rings -= 30;
                break;
            case 2:
                airToAdd /= 2;
                player->changeInAir_gain += airToAdd;
                player->rings -= 25;
                break;
        }
        Player_CoverF_UpdateStats(player, archetype);

        player->gearStats[0].boostCost = player->gearptr->levelStats[0].boostCost;
        player->gearStats[1].boostCost = player->gearptr->levelStats[1].boostCost;
        player->specialFlags &= ~berserkerEffect;

        Player_CreateCoverFParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);

    } else if (player->input->toggleFaceButtons & DPadUp) {
        // Solution Battle (SB)
        archetype = 3;
        if (player->characterArchetype == BoostArchetype)
        {
            if (player->state != RailGrind && !(player->movementFlags & boosting) && player->state != AttackingPlayer) player->unkF40 = 0;
        } else if (!(player->movementFlags & boosting) && player->state != AttackingPlayer) player->unkF40 = 0;

        switch (Player_CoverF_ChangeArchetype(player, archetype)) {
            default:
                return;
            case 1:
                player->changeInAir_gain += airToAdd;
                player->rings -= 30;
                break;
            case 2:
                airToAdd /= 2;
                player->changeInAir_gain += airToAdd;
                player->rings -= 25;
                break;
        }
        Player_CoverF_UpdateStats(player, archetype);

        player->gearStats[0].boostCost = 20000;
        player->gearStats[1].boostCost = 25000;
        player->specialFlags |= berserkerEffect;

        Player_CreateCoverFParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);

    }
}