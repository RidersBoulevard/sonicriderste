#include "riders/player.hpp"
#include "riders/stage.hpp"

constexpr std::array<f32, 17> data_C241155C = {
    0.0f, // test stage
    0.05f, // metal city
    0.15f, // splash canyon
    0.05f, // egg factory
    0.0005f, // green cave
    0.05f, // sand ruins
    0.075f, // babylon garden
    0.0f, // digital dimension
    0.15f, // sega carnival
    0.13f, // night chase
    0.05f, // red canyon
    0.05f, // ice factory
    0.0f, // white cave
    0.0f, // dark desert
    0.15f, // sky road
    0.15f, // babylon guardian
    0.15f, // sega illusion
};

ASMUsed void Player_GrindRailFastFall(Player *player) {
    u32 stage = CurrentStage;
    f32 extraGravity;
    if (player->state == PlayerState::Fall && player->previousState == PlayerState::RailGrind)
    {
        extraGravity = data_C241155C[stage];
        if (stage == GreenCave)
        {
            if (player->railID == 0x1) // second rail
            {
                extraGravity += 0.05f;
            }
        } else if (stage == DarkDesert && player->railID == 0x1) extraGravity += 0.05f;

        player->gravity += extraGravity;
    }
}