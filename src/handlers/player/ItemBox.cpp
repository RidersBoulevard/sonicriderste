#include "ItemBox.hpp"
#include "antiitemcamp.hpp"
#include "macros.h"
#include "mechanics/speed_shoes.hpp"

void ItemBox_GiveSpeedShoes(Player &player) {
    get_speed_shoes_data(player).has_speed_shoes = true;
    Player_HandleSpeedShoes(player);
    // Sound::PlaySound(Sound::ID::IDKSFX, 0x3d);
    if constexpr (enable_speed_shoes_recolor) {
        for (auto &material: gpsaObject_Player[player.index]->materials()) {
            auto &[r, g, b] = material.materialData->color.diffuse;
            b = g = 0.f;
        }
    }
}

ASMDefined void ItemBox_GiveItem_Cpp(Player &player, ItemID item) {
    // WARNING: This function currently requires it to be compiled down to tail calls!
    //
    // (All function calls except for AntiItemCampHandler are not actual functions,
    // but branches in the middle of another function)
    //
    // If you don't know what this constraint means, best not to touch this function.
    item = AntiItemCampHandler(player, item);

    asm volatile("mr r15, r3\n"); // Move "item" to r15, so that the other function has the correct item ID

    switch (item) {
        using enum ItemID;
        case FiveRings:
        case TenRings:
        case HundredRings:
            // Both IDs use this function, ItemBox_Ring_Ten() contains the necessary exception for FiveRings
            // If HundredRings happens, it'll only give 10 rings.
            ItemBox_Ring_Ten(0, player.aiControl);
            return;
        case TwentyRings: ItemBox_Ring_Twenty(0, player.aiControl); return;
        case ThirtyRings: ItemBox_Ring_Thirty(0, player.aiControl); return;
        case ThirtyAir: ItemBox_Air_Thirty(0, player.aiControl); return;
        case FiftyAir: ItemBox_Air_Fifty(0, player.aiControl); return;
        case HundredAir: ItemBox_Air_Hundred(0, player.aiControl); return;
        case SpeedShoes: ItemBox_GiveSpeedShoes(player); break;
        case Magnet: ItemBox_Magnet(0, player.aiControl); return;
        case Invincibility: ItemBox_Invincibility(0, player.aiControl); return;
        case Bomb: ItemBox_Bomb(0, player.aiControl); return;
        case BallAndChain: ItemBox_BallAndChain(0, player.aiControl); return;
        default: break;
    }

    ItemBox_ShowUI();
}

