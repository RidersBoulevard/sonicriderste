#pragma once
#include "riders/object.hpp"
#include "riders/player.hpp"

ASMDefined void ItemBox_GiveItem_Cpp(Player &player, ItemID item);

void ItemBox_GiveSpeedShoes(Player &player);

ASMDefined void ItemBox_ShowUI();
ASMDefined void ItemBox_ShowItem(Player &player, ItemID item, u16 duration = 0x3c);

// All these labels will have a dummy first argument and the 2nd argument as player.aiControl,
// as when jumping to these labels, r4 has to be set to player.aiControl.
ASMDefined void ItemBox_Ring_Ten([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Ring_Twenty([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Ring_Thirty([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Air_Thirty([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Air_Fifty([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Air_Hundred([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Magnet([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Invincibility([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_Bomb([[maybe_unused]] u32 dummy, bool aiControl);
ASMDefined void ItemBox_BallAndChain([[maybe_unused]] u32 dummy, bool aiControl);

ASMDefined void ItemBox_SpeedShoes_DrawUI();
ASMDefined void ItemBox_SpeedShoes_PlaySound(u32 = 0);
ASMDefined void ItemBox_SpeedShoes_PlayADX(u32 = 0);
ASMDefined void ApplyCameraPreset(u8 index, u16 preset_id);

