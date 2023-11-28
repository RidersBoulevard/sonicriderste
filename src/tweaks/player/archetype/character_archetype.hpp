#pragma once

#include "riders/character.hpp"
#include "riders/player.hpp"

constexpr std::array<CharacterArchetypes, TotalCharacterAmount> AllCharacterArchetypes = {
		LateBooster, // sonic
		CombatArchetype, // tails
		CombatArchetype, // knuckles
		DriftArchetype, // amy
		TopSpeedArchetype, // jet
		TopSpeedArchetype, // storm
		LateBooster, // wave
		AllRounder, // eggman
		AllRounder, // cream
		DriftArchetype, // rouge
		CombatArchetype, // shadow
		SuperSonicArchetype, // super sonic
		TopSpeedArchetype, // nights
		LateBooster, // aiai
		AllRounder, // ulala
		NoTypeArchetype, // e10g
		DriftArchetype, // e10b/e10r
		TricksterArchetype, // silver
		BoostArchetype, // metal sonic
		AllRounder, // emerl
		TricksterArchetype, // blaze
		BoostArchetype, // chaos
		TricksterArchetype, // tikal
};

ASMUsed void Player_CharacterArchetype(Player *player);