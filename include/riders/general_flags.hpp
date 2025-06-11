/** @file general_flags.hpp
 * Header file that contains definitions for all general sorts of flags for a player.
 */

#pragma once

#include "../types.hpp"

/// Used for Player::movementFlags and Player::last_movementFlags
enum class MovementFlags : u32{
	None					= 0,
	drifting 				= 1<<14,
	boosting 				= 1<<10,
	railLink 				= 1<<16,
	braking 				= 1<<8,
	TurbulenceLRExit		= 1<<18
};
ENUM_FLAG_OPERATORS(MovementFlags) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/// Used for Player::flags
enum class PlayerFlags : u32{
	None					= 0,
	Unknown					= 1 << 2,
    WallBonk				= 1 << 5,
	Unknown2				= 1 << 8,
	OnTurbulence			= 1 << 11,
	TurbulenceLRExit		= 1 << 14,
	Unknown3				= 1 << 19,
	ItemBoxCooldown			= 1 << 21,
	InAPit					= 1 << 25,
};
ENUM_FLAG_OPERATORS(PlayerFlags); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/// Used for Player::statusEffectFlags
enum class PlayerStatus : u32 {
	NoStatus 				= 0,
	BallAndChainStatus 		= 1 << 0,
	MagnetStatus			= 1 << 2,
	InvincibilityStatus 	= 1 << 4,
};
ENUM_FLAG_OPERATORS(PlayerStatus); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/// Used for Player::animationFlags
enum class AnimationFlags : u32 {
    None					= 0,
    AnimationEnd			= 1 << 4, ///< Set when the currently playing animation just ended.
    AnimationPaused			= 1 << 5,
};
ENUM_FLAG_OPERATORS(AnimationFlags); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)