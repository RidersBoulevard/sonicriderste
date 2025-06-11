#include "boundingbox.hpp"

#include <riders/stage.hpp>

ASMUsed float Player_BoundingBoxAdjustments(Player &player) {
    // if on a halfpipe, change bounding box data vertically in data.s
    // Known places this should be changed if condition is true:
    // Halfpipes
    // Illusion top path
    // Splash manual
    if (player.currentCollision == nullptr) return -1.5f;

    bool splashCanyonManualCheck = CurrentStage == SplashCanyon
    && player.currentCollision->properties.type == CollisionProperties::ManualRamp
    && player.death_spawnPoint == 8;

    // For some reason, this is a wall sometimes
    bool carnivalHalfpipeCheck = CurrentStage == SegaCarnival
    && (player.currentCollision->properties.type == CollisionProperties::HalfpipeRamp || player.currentCollision->properties.type == CollisionProperties::Wall)
    && (player.death_spawnPoint >= 27 && player.death_spawnPoint <= 30);

    bool eggfacHalfpipeCheck = CurrentStage == EggFactory
    && (player.currentCollision->properties.type == CollisionProperties::HalfpipeRamp)
    && (player.death_spawnPoint >= 40 && player.death_spawnPoint <= 43);

    // 7 for first pipe
    bool gardenHalfpipeCheck = CurrentStage == BabylonGarden
    && (player.currentCollision->properties.type == CollisionProperties::HalfpipeRamp)
    && (player.death_spawnPoint == 7 || (player.death_spawnPoint >= 57 && player.death_spawnPoint <= 59));

    bool illusionTopPathCheck = CurrentStage == SegaIllusion && player.death_spawnPoint == 22;

    if(splashCanyonManualCheck || carnivalHalfpipeCheck || eggfacHalfpipeCheck || gardenHalfpipeCheck || illusionTopPathCheck) return -1.0f;
    return -1.5f;
}
