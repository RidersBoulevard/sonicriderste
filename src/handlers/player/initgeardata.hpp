#pragma once

#include "riders/character.hpp"

ASMDefined f32 gf32_SystemHzAdd1;

struct LevelTypeStats {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
};

struct TypeStats {
	std::array<LevelTypeStats, 3> stats;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
};

constexpr std::array<TypeStats, 3> StatPresets = {{
		{{{{0.462962955236435f, 0.717592597007751f, 0.75f, 0.00385802472010255f, 0.000848765426781028f, 0.000308641989249736f, 50.0f}, {0.462962955236435f, 0.717592597007751f, 0.810185194015503f, 0.00385802472010255f, 0.000848765426781028f, 0.000308641989249736f, 70.0f}, {0.532407402992249f, 0.736111104488373f, 0.870370388031006f, 0.00441358052194119f, 0.000563271576538682f, 0.000540123437531292f, 70.0f}}}, 0.800000011920929f, 0.0253072753548622f, 0.00959931127727032f, 0.00261799385771155f},
		{{{{0.648148119449615f, 0.671296298503876f, 0.717592597007751f, 0.00555555615574121f, 0.000254629616392776f, 0.000123456789879128f, 40.0f}, {0.648148119449615f, 0.671296298503876f, 0.777777791023254f, 0.00555555615574121f, 0.000794753083027899f, 0.000123456789879128f, 60.0f}, {0.671296298503876f, 0.717592597007751f, 0.837962985038757f, 0.00624999590218067f, 0.00115740741603076f, 0.000231481477385387f, 60.0f}}}, 0.899999976158142f, 0.0253072753548622f, 0.00959931127727032f, 0.00261799385771155f},
		{{{{0.370370358228683f, 0.671296298503876f, 0.731481492519379f, 0.0030864195432514f, 0.000895061704795808f, 0.000578703708015382f, 80.0f}, {0.462962955236435f, 0.671296298503876f, 0.791666686534882f, 0.00385802472010255f, 0.000640432117506862f, 0.000192901221453212f, 80.0f}, {0.462962955236435f, 0.671296298503876f, 0.851851880550385f, 0.00385802472010255f, 0.000640432117506862f, 0.000578703708015382f, 90.0f}}}, 0.800000011920929f, 0.0253072753548622f, 0.00959931127727032f, 0.00261799385771155f},
}};

constexpr std::array<f32, Character::Total> CharacterWeights = {
		1.1F, //sonic
		0.9F, //tails
		1.4F, //knuckles
		1.05F,//amy
		1,    //jet
		1.45F,//storm
		0.95F,//wave
		1.55F,//eggman
		0.8F, //cream
		1.3F, //rouge
		1.15F,//shadow
		0.75F,//super sonic
		0.85F,//nights
		1.02F,//aiai
		1.2F, //ulala
		1.25F,//e10g
		1.35F,//e10r
		1.05F,// silver
		1.1F, // metal sonic
		1.15F,// emerl
		1.05F,// blaze
		1.4F, // chaos
		0.9F, // tikal
};

constexpr std::array<f32, 3> BoostArchetypeBoostSpeeds = {
		pSpeed(1.1f), pSpeed(2.1f), pSpeed(3.1f)
};

constexpr std::array<f32, 3> EmerlTopSpeedBonus = {
		pSpeed(20.0f), pSpeed(15.0f), pSpeed(5.0f)
};

constexpr std::array<f32, std::to_underlying(CharacterArchetype::Count)> Archetype_TopSpeedBonus = {
		pSpeed(5.0f), // all rounder (+5)
		pSpeed(0), // late booster
		pSpeed(10.1f), // top speed (+10)
		pSpeed(0), // combat
		pSpeed(0), // drift
		pSpeed(14.0f), // no type (+14)
		pSpeed(0), // super sonic
		pSpeed(0), // boost
		pSpeed(0), // trickster
		pSpeed(0), // none archetype
        pSpeed(0), // Windcatcher
        pSpeed(0), // Turning
        pSpeed(0), // Mechanic
};