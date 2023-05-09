#include <ppu_intrinsics.h>
#include "context.hpp"

constexpr u8 ItemRNGChange_level3AirGearRNG[0xB] = {
40, 0, 0, 0, // 40% 10 ring box
30, 0, 30, 0, // 30% 50 air, 30% speed shoes
0, 0, 0};

constexpr u8 ItemRNGChange_RingGearRNG[0xB] = {
60, 0, 0, 0, // 60% 10 ring box
20, 0, 20, 0, // 20% 50 air, 20% speed shoes
0, 0, 0};

ASMUsed const void* ItemRNGChange(Player *player, const u8 rngChances[], u32 currentItem) {
    f32 distance;

    u32 currentItemBool = (currentItem == 0) ? TRUE : FALSE;
    currentItem = 0; // r26
    
    if (currentItemBool) {
        if (CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix) {
            if (player->placement != 0) {
                u32 behindPlacement = player->placement - 1;
                Player *player2 = players.data();
                
                while (player2->placement != behindPlacement) {
                    player2++;
                }

                distance = __fabsf(player->stageProgress - player2->stageProgress);
                if (distance > 3000) {
                    rngChances += 0xB;
                    goto END;
                }
            }

            if (!(player->specialFlags & ringGear)) {
                if (player->level >= 2 && player->placement == 1) {
                    rngChances = ItemRNGChange_level3AirGearRNG;
                }
            }
            else {
                if (player->placement_counter <= 1) {
                    rngChances = ItemRNGChange_RingGearRNG;
                }
            }
        }
    }

END:
    return &rngChances[currentItem]; // return u8** ?
}