/** @file controller.hpp
 * Header file for handling anything regarding player controller input.
 */

#pragma once

#include "flags.hpp"
#include "globalDefs.hpp"

enum class Buttons : u32{
	Nothing 			= 0,
	A 					= 1<<0,
	B 					= 1<<1,
	X 					= 1<<2,
	Y 					= 1<<3,
	DPadUp 				= 1<<4,
	DPadDown 			= 1<<5,
	DPadLeft 			= 1<<6,
	DPadRight 			= 1<<7,
	Start 				= 1<<8,
	L 					= 1<<9,
	R 					= 1<<10,
	Z 					= 1<<11,
	LStickUp 			= 1<<12,
	LStickDown 			= 1<<13,
	LStickLeft 			= 1<<14,
	LStickRight 		= 1<<15,
};
ENUM_FLAG_OPERATORS(Buttons) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

struct Controller {
	// 0x30 in size
	u32 timeSinceLastInput; ///< resets to 0 every time an input is recorded, otherwise counts up infinitely
	u32 unk4;
	Flag<Buttons> holdFaceButtons; ///< currently held buttons get stored here every frame
	Flag<Buttons> toggleFaceButtons; ///< buttons that were pressed only get stored for a frame, doesn't detect if you're holding it
	fillerData<0x8> filler;
	s8 leftStickHorizontal; ///< from -100 to 100
	s8 leftStickVertical; ///< from -100 to 100
	fillerData<0x2> filler2;
	s8 rightStickHorizontal; ///< from -100 to 100
	s8 rightStickVertical; ///< from -100 to 100
	u8 port;
	fillerData<0x5> filler3;
	u32 initStatus2; ///< this will be 0xFFFFFFFF or -1 if not plugged in
	bool initStatus; ///< true if plugged in
	bool unk29; ///< always 1?
	fillerData<0x6> filler4;

	[[nodiscard]] inline auto getFaceButtons() const{
		return holdFaceButtons | toggleFaceButtons;
	}
};
static_assert(sizeof(Controller) == 0x30);

/// Contains all 4 controllers.
ASMDefined std::array<Controller, MaxControllerCount> GameControllers;