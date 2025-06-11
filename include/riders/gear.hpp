/** @file gear.hpp
 * Header file for dealing with anything regarding Extreme Gears.
 */

#pragma once

#include <containers/vector3.hpp>

#include "../flags.hpp"
#include "../types.hpp"

/**
 * Contains all of the game's Extreme Gears.
 */
namespace ExtremeGear{
	enum Enum : u8 {
		Default, HighBooster, AutoSlider, PowerfulGear, Fastest, TurboStar, SpeedBalancer, BlueStar2, Access, Beginner,
		Accelerator, TrapGear, LightBoard, SlideBooster, Legend, MagicCarpet, AirBroom, Hovercraft, ChaosEmerald, Faster,
		Gambler, PowerGear, OpaOpa, TheCrazy, Berserker, ERider, AirTank, HeavyBike, Destroyer, Omnipotence, CoverS, CoverF,
		CoverP, HangOn, SuperHangOn, Darkness, Grinder, AdvantageS, AdvantageF, AdvantageP, Cannonball,

		GunGear,

		TotalGearAmount,
		Invalid = 0xFF
	};

    constexpr auto BIKE_COUNT = 10;
}

enum class GearType : u8 {
	Board, Skates, Bike, EggmanType
};

enum class Type : u8{
	None = 0,
	Speed = 1<<0,
	Fly = 1<<1,
	Power = 1<<2,
	All = Speed | Fly | Power
};
ENUM_FLAG_OPERATORS(Type) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/* 0x9D4 */
enum class SpecialFlags : u32{
	noSpecialFlags 				= 0,
	alwaysIgnoreTurbulence 		= 1<<0,
	legendEffect 				= 1<<1,
	alwaysOnIce 				= 1<<2,
	unk 						= 1<<3,
	noBoosting 					= 1<<4,
	autoDrift 					= 1<<5,
	noSpeedLossUphill 			= 1<<6,
	tornadoBoost 				= 1<<7,
	noSpeedLossChargingJump 	= 1<<8,
	firstPlaceDoubleRings 		= 1<<9,
	ringGear 					= 1<<10,
	disableAttacks 				= 1<<11,
	berserkerEffect 			= 1<<12,
	noTypeShortcuts 			= 1<<13,
	noPits 						= 1<<14,
	thirtyPercentAir 			= 1<<15,
	fiftyPercentAir 			= 1<<16,
	iceImmunity 				= 1<<17,
	lightBoardEffect 			= 1<<18,
	stickyFingers 				= 1<<19,
	// custom
	lowBoost 					= 1<<20,
	lowerDecel 					= 1<<21,
	noBoostChain 				= 1<<22,
	noSpeedLossTurning 			= 1<<23,
	ignoreOffroad    			= 1<<24, // This special flag is used in assembly. If you change this special flag value, make sure to switch it around in text.s
	moneyCrisis					= 1<<25
};
ENUM_FLAG_OPERATORS(SpecialFlags) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

/// Contains the gear's stats for each level.
struct GearLevelStats {
	/* 0x0 */ s32 maxAir;
	/* 0x4 */ s32 passiveAirDrain;
	/* 0x8 */ s32 driftingAirCost;
	/* 0xC */ s32 boostCost;
	/* 0x10 */ s32 tornadoCost;
	/* 0x14 */ f32 driftDashSpeed;
	/* 0x18 */ f32 boostSpeed;
};
static_assert(sizeof(GearLevelStats) == 28);

struct GearExhaustBones {
    std::array<u8, 4> bone;
};

struct GearExhaustTrail {
    std::array<f32, 4> width;
    fillerData<0x30> fillerData1;
    std::array<Vector3F, 4> offset;
    std::array<GearExhaustBones, 2> boneBinding;
    f32 trailLength; /// only used if certain exhaust trail flags are set
    u32 unknownFlags;
    u8 unknown_Sometimes2;
    u8 exhaustTrailCount;
    u8 unknown3;
    u8 unknown4;
};
static_assert(sizeof(GearExhaustTrail) == 0x84);

struct Gear {
	/* 0x0 */ u32 useFlags; ///< which characters can use the gear. these values are bitwise.
	/* 0x4 */ u8 type;
	/* 0x5 */ u8 model; ///< the ID to use for loading the extreme gear model
	/* 0x6 */ u16 costInGearShop;
	/* 0x8 */ u32 unk8;
	/* 0xC */ f32 acceleration;
	/* 0x10 */ f32 topSpeed;
	/* 0x14 */ f32 offRoadSpeed;
	/* 0x18 */ f32 speedHandlingMultiplier;
	/* 0x1C */ f32 weight;
	/* 0x20 */ fillerData<0x3> unk20;
	/* 0x23 */ Flag<Type> extraTypeAttributes;
	/* 0x24 */ f32 turningSpeedLoss;
	/* 0x28 */ f32 handling;
	/* 0x2C */ f32 backAxelHandling;
	/* 0x30 */ f32 frontAxelHandling;
	/* 0x34 */ f32 driftRadius;
	/* 0x38 */ f32 driftRotation;
	/* 0x3C */ f32 backAxelDriftRotation;
	/* 0x40 */ f32 frontAxelDriftRotation;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ s32 driftDashFrames;
	/* 0x54 */ f32 trickAirGainMultiplier;
	/* 0x58 */ f32 shortcutAirGainMultiplier;
	/* 0x5C */ f32 QTEAirGainMultiplier;
	/* 0x60 */ Flag<SpecialFlags> specialFlags;
	/* 0x64 */ f32 jumpChargeCostMultiplier;
	/* 0x68 */ std::array<GearLevelStats, 3> levelStats;
	/* 0xBC */ s8 tempoStat;
	/* 0xBD */ s8 efficiencyStat;
	/* 0xBE */ s8 combatStat;
	/* 0xBF */ s8 weightStat;
	/* 0xC0 */ u32 unkC0;
	/* 0xC4 */ u32 unkC4;

    /// First struct in the array is for exhaust trail details whilst cruising, second struct is for whilst tricking
	/* 0xC8 - 0x1D0 */ std::array<GearExhaustTrail, 2> exhaustTrails;
};
static_assert(sizeof(Gear) == 0x1D0);

ASMDefined std::array<Gear,ExtremeGear::TotalGearAmount> Gears;