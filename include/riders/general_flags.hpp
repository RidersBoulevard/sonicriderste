/** @file general_flags.hpp
 * Header file that contains definitions for all general sorts of flags for a player.
 */

#pragma once

#include "../types.hpp"

/// Used for Player::movementFlags and Player::last_movementFlags
enum MovementFlags : u32{
	NoMovementFlags				= 0,
	drifting 					= 1<<14,
	boosting 					= 1<<10,
	railLink 					= 1<<16,
	braking 					= 1<<8,
	TurbulenceLRExit 			= 1<<18
};
ENUM_FLAG_OPERATORS(MovementFlags) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/// Used for Player::flags
enum PlayerFlags : u32{
	NoFlags						= 0,
	UnknownPlayerFlag			= 1 << 2,
    WallBonk                    = 1 << 5,
	UnknownPlayerFlag2			= 1 << 8,
	OnTurbulenceFlag 			= 1 << 11,
	TurbulenceLRExitFlag 		= 1 << 14,
	ItemBoxCooldownFlag 		= 1 << 21,
	InAPit 						= 1 << 25,
};
ENUM_FLAG_OPERATORS(PlayerFlags); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/// Used for Player::statusEffectFlags
enum PlayerStatus : u32 {
	NoStatus 					= 0,
	BallAndChainStatus 			= 1 << 0,
	InvincibilityStatus 		= 1 << 4,
};
ENUM_FLAG_OPERATORS(PlayerStatus); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/// Used for Player::animationFlags
enum class AnimationFlags : u32 {
    None                        = 0,
    AnimationEnd                = 1 << 4, ///< Set when the currently playing animation just ended.
    AnimationPaused             = 1 << 5,
};
ENUM_FLAG_OPERATORS(AnimationFlags); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)