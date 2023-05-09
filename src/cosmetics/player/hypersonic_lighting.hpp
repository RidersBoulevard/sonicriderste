#pragma once

#include "context.hpp"

global void *lbl_1000DFE4; // pointer to character model data

void HyperSonic_RainbowLighting(Player *player);

struct Lighting {
	char filler[0x14];
	f32 red1;
	f32 green1;
	f32 blue1;
	f32 red2;
	f32 green2;
	f32 blue2;
};

struct RGBModule {
	f32 R;
	f32 G;
	f32 B;
};

struct HyperSonic_ColorState {
	u8 state;
	u8 transitionFrames;
};
