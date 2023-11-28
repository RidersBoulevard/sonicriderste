/** @file character.hpp
 * Header file for definitions related to the characters in the game.
 */

#pragma once

#include "../types.hpp"
#include "../globalDefs.hpp"

enum CharacterEnums : u8 {
	Sonic, Tails, Knuckles, Amy, Jet, Storm, Wave, Eggman, Cream, Rouge, Shadow, SuperSonic,
	Nights, Aiai, Ulala, E10G, E10R, Silver, MetalSonic, Emerl, Blaze, Chaos, Tikal,
	// Add new characters above
	TotalCharacterAmount
};
static_assert(TotalCharacterAmount < 32, "Too many characters! This will break Gear::useFlags!");

struct Character {
	enum TypeEnum : u8{
		SpeedType,
		FlyType,
		PowerType,
		NoType
	};
	TypeEnum type;
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
static_assert(sizeof(Character) == 32);
ASMDefined std::array<Character, TotalCharacterAmount> Characters;

/// Holds all existing distinct character archetypes.
enum CharacterArchetypes : u8{
	NoEXLoadArchetype = 0xFF,
	AllRounder = 0,
	LateBooster,
	TopSpeedArchetype,
	CombatArchetype,
	DriftArchetype,
	NoTypeArchetype,
	SuperSonicArchetype,
	BoostArchetype,
	TricksterArchetype,
	NoneArchetype,
	Windcatcher,
	Turning,
	Mechanic,
	ArchetypeCount,
};