#pragma once

#include "riders/object.hpp"
#include "riders/player.hpp"

/**
 * SetPlayerState's asm declared things
 */
ASMDefined void func_ClearPlayerMotionTable(Player &player);
ASMDefined void func_GetDirectTrickBonus(Player &player, u32);
ASMDefined void func_StartPlayerMotionTable(Player &player);
ASMDefined void func__RegistPlayerMotionTable(Player &player, u32, u32, f32, f32);
ASMDefined void lbl_0009195C(void *);
ASMDefined void lbl_000FC114(Player &player);
ASMDefined void lbl_00187ED0(Player &player);
ASMDefined void lbl_Player_BoostEndFunction(Player &player);
ASMDefined void func_Particle_Task();

ASMDefined void *gcosNnSystemVecZeroFast;
ASMDefined vf32 gf32_SystemHzAdd1;
ASMDefined vs32 gpsTexList_Particle;
ASMDefined volatile MagicCarpetParticle gsParFullParam_MagicCarpet;
ASMDefined vs32 lbl_10087C40;
// End of ASM stuff

ASMUsed void func_SetPlayerActionType(Player &player, u32 state);