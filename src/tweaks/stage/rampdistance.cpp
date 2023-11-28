#include "riders/player.hpp"
#include "riders/stage.hpp"

constexpr m2darray<f32, TotalStageAmount, 2> SideflipRampDistances = {{
		// first float is for forward flips, second one is for back flips
		{0.0f, 0.0f}, // test stage
		{0.0f, 0.0f}, // metal city
		{0.0f, 0.0f}, // splash canyon
		{0.0f, 0.1f}, // egg factory
		{0.0f, 0.0f}, // green cave
		{0.0f, 0.1f}, // sand ruins
		{0.0f, 0.0f}, // babylon garden
		{0.0f, 0.0f}, // digital dimension
		{0.0f, 0.0f}, // sega carnival
		{0.0f, 0.0f}, // night chase
		{0.0f, 0.0f}, // red canyon
		{0.0f, 0.01f},// ice factory
		{0.0f, 0.0f}, // white cave
		{0.2f, 0.0f}, // dark desert
		{0.0f, 0.0f}, // sky road
		{0.0f, 0.0f}, // babylon guardian
		{0.0f, 0.1f}, // sega illusion
}};

ASMUsed f32 Player_SideflipRampDistance(Player *player, f32 currentDistance) {
    if (player->trickTrajectory == 0x3) {
        // side flip
        currentDistance += SideflipRampDistances[CurrentStage][player->rampType];
    } else if (player->trickTrajectory == 0x1) {
        // A button without charging
        switch (CurrentStage) {
            case SegaIllusion:
                if (player->rampType == 0x1) {
                    // back flip ramp
                    currentDistance += 0.15f;
                }
                break;
            case EggFactory:
                if (player->rampType == 0x1) {
                    // back flip ramp
                    currentDistance += 0.05f;
                }
                break;
            case DarkDesert:
            case SegaCarnival:
                if (player->rampType == 0) {
                    // front flip ramp
                    currentDistance += 0.1f;
                }
                break;
			default:
				break;
        }
    }

    return currentDistance;
}