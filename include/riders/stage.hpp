/** \file stage.hpp
 * Header file for definitions related to in-game stages.
 */

#pragma once

#include "../types.hpp"
#include "globalDefs.hpp"
#include "riders/object.hpp"

enum Stage {
	TestStage, MetalCity, SplashCanyon, EggFactory, GreenCave, SandRuins, BabylonGarden,
	DigitalDimension, SegaCarnival, NightChase, RedCanyon, IceFactory, WhiteCave, DarkDesert,
	SkyRoad, BabylonGuardian, SegaIllusion,

	TotalStageAmount
};

constexpr auto TotalHeroStageCount = 8;
ASMDefined vu32 CurrentStage;

USED inline bool gStGateForce;
USED inline ObjectNode *gStGateTask;

inline void forceStartGate(bool enable) {
    gStGateForce = enable;

    if (enable) {
        gStGateTask->state = 4;
    }
}