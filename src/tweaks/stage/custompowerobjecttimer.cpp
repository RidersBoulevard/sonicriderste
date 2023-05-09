#include "context.hpp"

ASMUsed u32 CustomPowerObjectTimer() {
    u32 powerObjectTimer = 600; // default timer
    u32 amountOfPowerTypesInGame = 0;
    u32 i;

    if (CurrentGameMode == FreeRace) {
        for (i = 0; i < 4; i++) {
            amountOfPowerTypesInGame += (players[i].typeAttributes & PowerType) ? TRUE : FALSE;
        }
    } else if (CurrentGameMode == WorldGrandPrix) {
        for (i = 0; i < 2; i++) {
            if (players[i].playerType == 0) { // non AI
                amountOfPowerTypesInGame += (players[i].typeAttributes & PowerType) ? TRUE : FALSE;
            }
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
    u32 i;

    if (CurrentGameMode == FreeRace) {
        for (i = 0; i < 4; i++) {
            amountOfPowerTypesInGame += (players[i].typeAttributes & PowerType) ? TRUE : FALSE;
        }
    }

    if (amountOfPowerTypesInGame >= 2) {
        powerObjectTimer = 30.0f; // 0.5 seconds
    }
    
    return powerObjectTimer;
}
