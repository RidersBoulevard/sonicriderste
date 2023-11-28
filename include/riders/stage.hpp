/** \file stage.hpp
 * Header file for definitions related to in-game stages.
 */

#pragma once

#include "../types.hpp"

enum Stage {
	TestStage, MetalCity, SplashCanyon, EggFactory, GreenCave, SandRuins, BabylonGarden,
	DigitalDimension, SegaCarnival, NightChase, RedCanyon, IceFactory, WhiteCave, DarkDesert,
	SkyRoad, BabylonGuardian, SegaIllusion,

	TotalStageAmount
};

constexpr auto TotalHeroStageCount = 8;
ASMDefined const vu32 CurrentStage;