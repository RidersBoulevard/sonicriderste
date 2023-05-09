#pragma once

#include "context.hpp"

global void func_Particle_Task();
global void* gpsTexList_Particle;
global void* lbl_001D8F58;

constexpr f32 CoverF_Speeds[3][2] = {
		// top speed, boost speed
		{pSpeed(240), pSpeed(220)}, // solution cruise
		{pSpeed(140), pSpeed(265)}, // solution turbo
		{pSpeed(200), pSpeed(240)}, // solution battle
};
constexpr f32 CoverF_SpeedsBoostArchetype[3][2] = {
		// top speed, boost speed
		{pSpeed(240), pSpeed(235)}, // solution cruise
		{pSpeed(140), pSpeed(270)}, // solution turbo
		{pSpeed(200), pSpeed(250)}, // solution battle
};

void Player_CoverF(Player *player);
