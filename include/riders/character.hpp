/** @file character.hpp
 * Header file for definitions related to the characters in the game.
 */

#pragma once

#include "../globalDefs.hpp"
#include "../types.hpp"

struct Character {
	enum Enum : u8 {
		Sonic, Tails, Knuckles, Amy, Jet, Storm, Wave, Eggman, Cream, Rouge, Shadow, SuperSonic,
		Nights, Aiai, Ulala, E10G, E10R, Silver, MetalSonic, Emerl, Blaze, Chaos, Tikal,
		// Add new characters above
		Total,
		Invalid = 0xFF
	};
	enum class Type : u8{
		Speed,
		Fly,
		Power,
		None
	};
	Type type;
	u8 statPreset;

    /**
     * Whether or not the character model should be drawn mirrored.
     * @note Some characters' attack animations rely on it being drawn mirrored/not mirrored.
     */
	bool mirroredModel;

	u8 model; ///< The ASCII character to use for loading this character's specific models.
	u32 unkx4;
	f32 cameraHeight;
	f32 speedMultiplier; ///< Works really weirdly. You shouldn't ever need to use this (used to be used to apply top speed but it came with extra side effects).

	/// Used to depict the star count on the CSS.
    struct lvlStats{
		s8 lvl1;
		s8 lvl2;
		s8 lvl3;
		s8 : 8; // padding
	};
	lvlStats dash;
	lvlStats limit;
	lvlStats power;
	lvlStats cornering;
};
static_assert(std::to_underlying(Character::Enum::Total) < 32, "Too many characters! This will break Gear::useFlags!");
static_assert(sizeof(Character) == 32);
ASMDefined std::array<Character, Character::Total> Characters;

/// Holds all existing distinct character archetypes.
enum class CharacterArchetype : u8{
	NoEXLoad = 0xFF,
	AllRounder = 0,
	LateBooster,
	TopSpeed,
	Combat,
	Drift,
	NoType,
	SuperSonic,
	Boost,
	Trickster,
	None,
	Windcatcher,
	Turning,
	Mechanic,
	Count,
};