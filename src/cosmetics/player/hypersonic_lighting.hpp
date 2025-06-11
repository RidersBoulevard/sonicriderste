#pragma once

#include "riders/player.hpp"

void HyperSonic_RainbowLighting(Player &player);

struct RGBModule {
	f32 R;
	f32 G;
	f32 B;

	constexpr auto operator<=>(const RGBModule &) const = default;
	constexpr RGBModule operator +(const RGBModule &rhs) const {
		return {R + rhs.R, G + rhs.G, B + rhs.B};
	}
	constexpr RGBModule& operator +(const RGBModule &rhs) {
		R += rhs.R;
		G += rhs.G;
		B += rhs.B;
		return *this;
	}
	constexpr RGBModule& operator +=(const RGBModule &rhs) {
		*this = *this + rhs;
		return *this;
	}
};

struct Lighting {
	fillerData<0x14> filler;
	std::array<RGBModule, 2> rgb_modules;
};

struct HyperSonic_ColorState {
	enum color : u8{
		red, blue, green, yellow, white, violet
	};
	color state;
	u8 transitionFrames;
};
