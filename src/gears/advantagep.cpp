#include "advantagep.hpp"

std::array<AdvantagePInfo, MaxPlayerCount> PlayerAdvantagePInfo;

void Player_AdvantageP(Player *player) {
    if (player->extremeGear != ExtremeGear::AdvantageP) {return;}
    AdvantagePInfo *advpInfo = &PlayerAdvantagePInfo[player->index];

    if (player->state == PlayerState::StartLine) {
        // Set TOP Gauge here
        const auto buttons = player->input->toggleFaceButtons.hasAny(Buttons::DPadLeft, Buttons::DPadUp, Buttons::DPadRight);
        switch (Buttons(buttons)) {
            case Buttons::DPadLeft:
                // Bottom 1/3rd
                if (advpInfo->TOP_Pos == 1) break;
                advpInfo->TOP_Pos = 1;
                PlayAudioFromDAT(Sound::SFX::AdvPTOP1);
                break;

            case Buttons::DPadUp:
                // Middle 1/3rd
                if (advpInfo->TOP_Pos == 2) break;
                advpInfo->TOP_Pos = 2;
                PlayAudioFromDAT(Sound::SFX::AdvPTOP2);
                break;

            case Buttons::DPadRight:
                // Top 1/3rd
                if (advpInfo->TOP_Pos == 3) break;
                advpInfo->TOP_Pos = 3;
                PlayAudioFromDAT(Sound::SFX::AdvPTOP3);
                break;

            default: break;
        }
    }
    
    if (advpInfo->TOP_Pos == 0 
        && (player->previousState == PlayerState::StartLine || player->previousState == PlayerState::StartLineShock)) {
        // If player did not choose an option, pick random one
        auto ranPos = lbl_RNG_Number(3);
        advpInfo->TOP_Pos = ranPos + 1;

        if (advpInfo->TOP_Pos != 0) {
            switch (advpInfo->TOP_Pos)
            {
                case 1:
                    PlayAudioFromDAT(Sound::SFX::AdvPTOP1);
                    break;
                case 2:
                    PlayAudioFromDAT(Sound::SFX::AdvPTOP2);
                    break;
                case 3:
                    PlayAudioFromDAT(Sound::SFX::AdvPTOP3);
                    break;
                default:
                    break;
            }
        }
    } 

    switch (advpInfo->TOP_Pos)
    {
        case 1:
            advpInfo->TOPGauge[0] = 0;
            advpInfo->TOPGauge[1] = player->gearStats[player->level].maxAir * 1/3;
            break;

        case 2:
            advpInfo->TOPGauge[0] = player->gearStats[player->level].maxAir * 1/3;
            advpInfo->TOPGauge[1] = player->gearStats[player->level].maxAir * 2/3;
            break;
    
        case 3:
            advpInfo->TOPGauge[0] = player->gearStats[player->level].maxAir * 2/3;
            advpInfo->TOPGauge[1] = player->gearStats[player->level].maxAir;
            break;
    default:
        break;
    }

    // Manage TOP Gauge here
    // Enters this statement if their current air is between the ranges set at the start line
    const bool inTopMode = player->currentAir >= advpInfo->TOPGauge[0] && player->currentAir <= advpInfo->TOPGauge[1];
    advpInfo->isInTopMode = inTopMode;
    if (inTopMode) {

        // First, passive air gain
        // Passive air gain stops once greater than the highest range
        player->gearStats[player->level].airDrain = player->gearptr->levelStats[player->level].passiveAirDrain * -1;

        // Second, increased "damage" (boost bonus)
        player->gearStats[player->level].boostSpeed = 
        (player->characterArchetype == CharacterArchetype::Boost) 
        ? player->gearptr->levelStats[player->level].boostSpeed + BoostArchetypeBoostSpeeds[player->level] + pSpeed(10.0f)
        : player->gearptr->levelStats[player->level].boostSpeed + pSpeed(10.0f);

        // Third, faster (air) meter buildup
        player->trickAirGainMultiplier = 1.15f;
        player->shortcutAirGainMultiplier = 1.15f;


    } else {
        // Set stats back to normal here if not in the range

        player->gearStats[player->level].airDrain = player->gearptr->levelStats[player->level].passiveAirDrain;

        if (!player->movementFlags.hasAny(MovementFlags::boosting)) {
        player->gearStats[player->level].boostSpeed = 
        (player->characterArchetype == CharacterArchetype::Boost) 
        ? player->gearptr->levelStats[player->level].boostSpeed + BoostArchetypeBoostSpeeds[player->level] 
        : player->gearptr->levelStats[player->level].boostSpeed;
        }

        player->trickAirGainMultiplier = 0.55f;
        player->shortcutAirGainMultiplier = 0.55f;

    }
}