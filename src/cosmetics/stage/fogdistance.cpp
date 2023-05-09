#include "context.hpp"

ASMUsed BOOL CustomStageFogDistance(f32 fogDistance[2]) {
    BOOL hasCustomFog = FALSE;

    switch (CurrentStage) {
        case GreenCave:
            hasCustomFog = TRUE;
            fogDistance[0] = 10.0f;
            fogDistance[1] = 200.0f;
            break;

        case IceFactory:
            hasCustomFog = TRUE;
            fogDistance[0] = 0.0f;
            fogDistance[1] = 0.0f;
            break;

        case RedCanyon:
            hasCustomFog = TRUE;
            fogDistance[0] = 20.0f;
            fogDistance[1] = 300.0f;
            break;

		default:
			break;
    }

    return hasCustomFog;
}