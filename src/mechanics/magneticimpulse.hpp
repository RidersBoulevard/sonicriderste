#pragma once

#include "containers/vector3.hpp"
#include "globalDefs.hpp"
#include "riders/player.hpp"
#include "types.hpp"

#include <array>
#include <ppu_intrinsics.h>

namespace MagneticImpulse {
	constexpr f32 NoTypeMultiplier = 2.0f;
	constexpr f32 DualTypeMultiplier = 0.8f;
	constexpr f32 AllTypeMultiplier = 0.6f;

	/// maximum amount of frames before MI reaches maximum state
	constexpr f32 MaximumCap = 1200.0f;
	/// how long max MI is guaranteed for
	constexpr f32 MaxMITime = 420.0f;
	/// the amount of frames it caps to when MI is lost
	constexpr f32 LossTimeCap = 900.0f;
	/// how many frames before MI turns off/on
	constexpr f32 BufferTime = 60.0f;
	// placeholder struct for storing MI data till I allocate stuff over to player data
	struct ImpulseData {
		f32 buffer = 0;
		f32 interrupt = 0;
		f32 maxMItimer = 0;
		f32 ringPowerObjTimer = 0;
		u32 afterburnerTimer = 0;
		u32 windcatcherTurbGain = 0;
		bool magneticImpulse = false;
	};
	extern std::array<ImpulseData, MaxPlayerCount> impulseData;
	extern std::array<Matrix3x3F, MaxPlayerCount> PlayerPositionRotationMatrices;

	void createGuaranteedMaxMIParticles(Player &player);
	f32 calculateMultiplier(Player *player, f32 value);
	f32 scaleUsingCurrentMI(Player &player, f32 value);
}// namespace MagneticImpulse
namespace MI = MagneticImpulse;// Alias the namespace into something shorter

void Player_MagneticImpulseTimer(Player &player);
