#include "globalDefs.hpp"
#include "riders/stage.hpp"
#include "riders/gamemode.hpp"

#include <span>

/**
 * Overrides fog distance for stages
 * @param fogDistancePtr array with 2 floats
 * @returns true if the fog was overridden, false otherwise
 */
ASMUsed bool CustomStageFogDistance(f32* const fogDistancePtr) {
	const std::span fogDistance = {fogDistancePtr, 2};
	switch(CurrentStage) {
		case GreenCave:
			fogDistance[0] = 10.0f;
			fogDistance[1] = 200.0f;
			return true;

		case IceFactory:
			fogDistance[0] = 0.0f;
			fogDistance[1] = 0.0f;
			return true;

		case RedCanyon:
		if (CurrentGameMode == CutsceneMode) return false;
			fogDistance[0] = 20.0f;
			fogDistance[1] = 300.0f;
			return true;

		default: break;
	}

	return false;
}