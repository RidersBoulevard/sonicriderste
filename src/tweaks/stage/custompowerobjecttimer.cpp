#include "riders/gamemode.hpp"
#include "riders/player.hpp"

ASMUsed u32 CustomPowerObjectTimer() {
    u32 powerObjectTimer = 600; // default timer
    u32 amountOfPowerTypesInGame = 0;

    if (CurrentGameMode == FreeRace) {
        for (u32 i = 0; i < 4; i++) {
            amountOfPowerTypesInGame += players[i].typeAttributes.hasAny(Type::Power) ? 1 : 0;
        }
    } else if (CurrentGameMode == WorldGrandPrix) {
        for (u32 i = 0; i < 2; i++) {
			if(players[i].playerType) { continue; }// Skip Ai players
			amountOfPowerTypesInGame += players[i].typeAttributes.hasAny(Type::Power) ? 1 : 0;
		}
    }

    if (amountOfPowerTypesInGame >= 2) {
        powerObjectTimer = 1; // instantenous
    }
    
    return powerObjectTimer;
}

ASMUsed f32 SegaIllusion_CustomPowerObjectTimer() {
    // this alters the timer for the pew pew guys after the QTE
    
    f32 powerObjectTimer = 300.0f; // default timer
    u32 amountOfPowerTypesInGame = 0;

    if (CurrentGameMode == FreeRace) {
        for (u32 i = 0; i < 4; i++) {
            amountOfPowerTypesInGame += players[i].typeAttributes.hasAny(Type::Power) ? 1 : 0;
        }
    }

    if (amountOfPowerTypesInGame >= 2) {
        powerObjectTimer = 30.0f; // 0.5 seconds
    }
    
    return powerObjectTimer;
}
