#pragma once

#include <type_traits>
#include <concepts>
#include <array>
#include <span>

using f32 = float;
using vf32 = volatile float;
using f64 = double;
using vf64 = volatile double;
using s32 = int;
using s16 = short;
using s8 = signed char;
using u32 = unsigned int;
using u8 = unsigned char;
using u16 = unsigned short;
using vs32 = volatile int;
using vs16 = volatile short;
using vs8 = volatile signed char;
using vu32 = volatile unsigned int;
using vu16 = volatile unsigned short;
using vu8 = volatile unsigned char;
using BOOL = bool;

using Task = void (*)();
#define TRUE true
#define FALSE false

#define ENUM_FLAG_OPERATORS(T)                                                                                                                                            \
    inline T operator~ (T a) { return static_cast<T>( ~static_cast<std::underlying_type<T>::type>(a) ); }                                                                       \
    inline T operator| (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) | static_cast<std::underlying_type<T>::type>(b) ); }                   \
    inline T operator& (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) & static_cast<std::underlying_type<T>::type>(b) ); }                   \
    inline T operator^ (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) ^ static_cast<std::underlying_type<T>::type>(b) ); }                   \
    inline T& operator|= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) |= static_cast<std::underlying_type<T>::type>(b) ); }   \
    inline T& operator&= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) &= static_cast<std::underlying_type<T>::type>(b) ); }   \
    inline T& operator^= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) ^= static_cast<std::underlying_type<T>::type>(b) ); }

template<typename T> concept arithmetic = std::integral<T> || std::floating_point<T>;

template<typename T> requires arithmetic<T>
constexpr f32 toFrames(const T &n){
	return static_cast<f32>(n) * 60.0F; // NOLINT(readability-magic-numbers)
}
//#define toFrames(n) ((n) * 60.0f)

// Disable's C++'s name mangling so ASM/C can see this as a normal symbol
#define global extern "C"

// Tells GCC that this item is actually used even if the C++ code doesn't use it
#define USED __attribute((used))

// Marks this function as one that asm calls
#define ASMUsed global USED

enum Type : u8{
	NoType = 0,
	SpeedType = 1<<0,
	FlyType = 1<<1,
	PowerType = 1<<2,
	AllType = SpeedType | FlyType | PowerType
};
ENUM_FLAG_OPERATORS(Type) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,readability-identifier-length)
//#define     SpeedType   0b0001UL
//#define     FlyType     0b0010UL
//#define     PowerType   0b0100UL
//#define		AllType		0b0111UL

            /* 0x9D4 */
enum SpecialFlags : u32{
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
};
ENUM_FLAG_OPERATORS(SpecialFlags) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,readability-identifier-length)
//#define	alwaysIgnoreTurbulence      0x00000001UL
//#define	legendEffect                0x00000002UL
//#define	alwaysOnIce                 0x00000004UL
//#define	unk                         0x00000008UL
//#define	noBoosting                  0x00000010UL
//#define	autoDrift                   0x00000020UL
//#define	noSpeedLossUphill           0x00000040UL
//#define	tornadoBoost                0x00000080UL
//#define	noSpeedLossChargingJump     0x00000100UL
//#define	firstPlaceDoubleRings       0x00000200UL
//#define	ringGear                    0x00000400UL
//#define	disableAttacks              0x00000800UL
//#define	berserkerEffect             0x00001000UL
//#define	noTypeShortcuts             0x00002000UL
//#define	noPits                      0x00004000UL
//#define	thirtyPercentAir            0x00008000UL
//#define	fiftyPercentAir             0x00010000UL
//#define	iceImmunity                 0x00020000UL
//#define	lightBoardEffect            0x00040000UL
//#define	stickyFingers               0x00080000UL
// custom
//#define	lowBoost                    0x00100000UL
//#define	lowerDecel                  0x00800000UL
//#define	noBoostChain				0x01000000UL
//#define	noSpeedLossTurning          0x04000000UL

    /* 0xA28 */
enum MovementFlags : u32{
	drifting = 1<<14,
	boosting = 1<<10,
	railLink = 1<<16,
	braking = 1<<8,
	TurbulenceLRExit = 1<<18
};
ENUM_FLAG_OPERATORS(MovementFlags) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,readability-identifier-length)
//#define     drifting    0x00004000UL
//#define     boosting    0x00000400UL
//#define     railLink    0x00010000UL
//#define     braking    0x00000100UL
//#define 	TurbulenceLRExit 	0x00040000UL

    /* 0xBA8 */
#define		ItemBoxCooldownFlag		0x00200000UL
#define 	TurbulenceLRExitFlag	0x00004000UL
#define 	OnTurbulenceFlag		0x00000800UL
#define		InAPit					0x02000000UL

    /* 0xBB0 */
#define		InvincibilityStatus		0x00000010UL
#define		BallAndChainStatus		0x00000001UL


#define		Wall					    0x00010000UL
#define		DeathPlane				    0x00020000UL
#define		OffRoadCollision		    0x00030000UL
#define		SpeedBoostCollision		    0x00040000UL
#define		ForwardFlipRampCollision	0x00050000UL
#define		BackFlipRampCollision		0x00060000UL
#define		ManualRampCollision			0x00090000UL
#define		FlightStateTrigger		    0x00110000UL
#define		OnIceCollision			    0x00230000UL

enum Buttons : u32{
	NothingHeld = 0,
	AButton = 1<<0,
	BButton = 1<<1,
	XButton = 1<<2,
	YButton = 1<<3,
	DPadUp = 1<<4,
	DPadDown = 1<<5,
	DPadLeft = 1<<6,
	DPadRight = 1<<7,
	StartButton = 1<<8,
	LButton = 1<<9,
	RButton = 1<<10,
	ZButton = 1<<11,
	LStickUp = 1<<12,
	LStickDown = 1<<13,
	LStickLeft = 1<<14,
	LStickRight = 1<<15,
};
ENUM_FLAG_OPERATORS(Buttons) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,readability-identifier-length)
//#define     AButton     0x00000001UL
//#define     BButton     0x00000002UL
//#define     XButton     0x00000004UL
//#define     YButton     0x00000008UL
//#define     DPadUp      0x00000010UL
//#define     DPadDown    0x00000020UL
//#define     DPadLeft    0x00000040UL
//#define     DPadRight   0x00000080UL
//#define     StartButton 0x00000100UL
//#define     LButton     0x00000200UL
//#define     RButton     0x00000400UL
//#define     ZButton     0x00000800UL


constexpr f32 SPEED_DIVISOR = 216.0F; // value used as a divisor to calculate player speed in game
constexpr f32 pSpeed(const f32 &n) {
	return n / SPEED_DIVISOR;
}
//#define     pSpeed(n)       ((n) / SPEED_DIVISOR)

enum CharacterEnums : u8 {
	Sonic, Tails, Knuckles, Amy, Jet, Storm, Wave, Eggman, Cream, Rouge, Shadow, SuperSonic,
	Nights, Aiai, Ulala, E10G, E10R, Silver, MetalSonic, Emerl, Blaze, Chaos, Tikal
	, TotalCharacterAmount
}; // Add new characters *before* TotalCharacterAmount enum

enum ExtremeGear : u8 {
	DefaultGear, HighBooster, AutoSlider, PowerfulGear, Fastest, TurboStar, SpeedBalancer, BlueStar2, Access, Beginner,
	Accelerator, TrapGear, LightBoard, SlideBooster, Legend, MagicCarpet, AirBroom, Hovercraft, ChaosEmerald, Faster,
	Gambler, PowerGear, OpaOpa, TheCrazy, Berserker, ERider, AirTank, HeavyBike, Destroyer, Omnipotence, CoverS, CoverF,
	CoverP, HangOn, SuperHangOn, Darkness, Grinder, AdvantageS, AdvantageF, AdvantageP, Cannonball, TotalGearAmount
};

enum GearType : u8 {
	Board, Skates, Bike
};

enum Stage {
	TestStage, MetalCity, SplashCanyon, EggFactory, GreenCave, SandRuins, BabylonGarden,
	DigitalDimension, SegaCarnival, NightChase, RedCanyon, IceFactory, WhiteCave, DarkDesert,
	SkyRoad, BabylonGuardian, SegaIllusion, TotalStageAmount
};

enum TrickRanks : u8 {
	CRank, BRank, ARank, APlusRank, SRank, SPlusRank, XRank
};

enum PlayerState : u8 {
	QTE = 0x0,
	StartLine = 0x1,
	Unknown = 0x2,
	Death = 0x3,
	Retire = 0x4,
	Cruise = 0x5,
	Jump = 0x6,
	Fall = 0x7,
	FrontflipRamp = 0x8,
	BackflipRamp = 0x9,
	UnknownTrick = 0xA,
	HalfPipeTrick = 0xB,
	ManualRamp = 0xC,
	TurbulenceTrick = 0xD,
	TurbulenceTrick2 = 0xE,
	PlayerCollide = 0xF, // i think this one triggers when someone bumps you?
	TurbulenceRide = 0x10,
	QTE2 = 0x11,
	RailGrind = 0x12,
	Fly = 0x13,
	AttackingPlayer = 0x15,
	AttackedByPlayer = 0x16,
	Stun = 0x17,
	Run = 0x19,
};

enum ItemID {
	TenRings, TwentyRings, ThirtyRings, ThirtyAir, FiftyAir, HundredAir, SpeedShoes, Magnet,
	Invincibility, Bomb, BallAndChain, RNG, HundredRings, MaxAir, FiveRings
};

enum ObjectTypes {
	ItemBox = 0x7DA, FlightRing = 0x848, DashPanel = 0x83E, WhiteCaveWeb = 0x1F40, Car = 0x13A8
};

enum GameModes {
	TitleScreen = 0x1, StoryMode = 0x64, MissionMode = 0xC8, TagMode = 0x12C, EmeraldChase = 0x190,
	BattleMode = 0x1F4, TimeTrial = 0x258, FreeRace = 0x2BC, WorldGrandPrix = 0x320
};

enum ObjectVisibility {
	GameCubeVisibility = 1 << 0,
	PlayStationVisibility = 1 << 1,
	XboxVisibility = 1 << 2,
	RaceVisibility = 1 << 3,
	TagVisibility = 1 << 4,
	SurvivalVisibility = 1 << 5,

	Mission1Visibility = 1 << 6,
	Mission2Visibility = 1 << 7,
	Mission3Visibility = 1 << 8,
	Mission4Visibility = 1 << 9,
	Mission5Visibility = 1 << 10,
	Mission6Visibility = 1 << 11,
	Mission7Visibility = 1 << 12
};

enum CustomObjectGroups {
	Object_SuperTailsRenderText = 0xF000, Object_EmerlTypeIcon
};

enum EXLoadModes {
	CharacterEXLoadMode, GearEXLoadMode
};

enum CharacterArchetypes {
	AllRounder,
	LateBooster,
	TopSpeedArchetype,
	CombatArchetype,
	DriftArchetype,
	NoTypeArchetype,
	SuperSonicArchetype,
	BoostArchetype,
	TricksterArchetype,
	NoneArchetype,
	ArchetypeCount,
    NoEXLoadArchetype = -1
};

// LLoadType = for ex loads that are accessed via the L button (cosmetic ex loads)
// EXLoadType = for ex loads that are accessed via the X button (gameplay altering ex loads)
// AllEXLoadTypes... = used for scrolling through both L and X loads
enum EXLoadTypes {
	NoneEXLoadType, LLoadType, EXLoadType, AllEXLoadTypesBackwards, AllEXLoadTypesForwards
};

enum GRStates {
	GR_None, GR_BumpingIntoPlayerState, GR_AttackingPlayerState, GR_TurbSlipstreamState
};

// add ex loads before the EXLoadCount enum please kthx
enum EXLoads {NoneEXLoad,
        PerfectNazo, E10REXLoad, DarkSonicEXLoad, HyperSonicEXLoad,
        StardustSpeederEXLoad, HyperHangOnEXLoad, TheProfessionalEXLoad,
        OllieKingGearEXLoad, WindmasterJetEXLoad, NeoMetalEXLoad, E99EXLoad,
        TheBeastEXLoad, E10BEXLoad, HatsuneMikuEXLoad, MetalSonicScarfEXLoad,
        RealaEXLoad, GonGonEXLoad, EXLoadCount};

struct Collision {
	union {
		f32 bound_x;
		u32 bound_xUL;
	};
	union {
		f32 bound_y;
		u32 bound_yUL;
	};
	union {
		f32 bound_z;
		u32 bound_zUL;
	};
	union {
		f32 bound_scale;
		u32 bound_scaleUL;
	};

	void* faceData;
	u32 properties;
};

struct Controller {
    // 0x30 in size
    u32 timeSinceLastInput; // resets to 0 every time an input is recorded, otherwise counts up infinitely
    u32 unk4;
    Buttons holdFaceButtons;
    Buttons toggleFaceButtons;
    char filler[0x8];
	s8 leftStickHorizontal;
	s8 leftStickVertical;
	char filler2[0x2];
    s8 rightStickHorizontal;
	s8 rightStickVertical;
    u8 port;
    char filler3[0x5];
    u32 initStatus2; // this will be 0xFFFFFFFF or -1 if not plugged in
    BOOL initStatus; // true if plugged in
    BOOL unk29; // always 1?
    char filler4[0x6];
};
static_assert(sizeof(Controller) == 0x30);

global std::array<Controller,4> GameControllers;

struct GearLevelStats {
    /* 0x0 */ s32 maxAir;
	/* 0x4 */ s32 passiveAirDrain;
	/* 0x8 */ s32 driftingAirCost;
	/* 0xC */ s32 boostCost;
	/* 0x10 */ s32 tornadoCost;
	/* 0x14 */ f32 driftDashSpeed;
	/* 0x18 */ f32 boostSpeed;
};

struct PlayerGearStats {
	// 0x38 in size
	/* 0x0 */ f32 baseTopSpeed;
	/* 0x4 */ f32 baseAccel;
	/* 0x8 */ f32 topSpeed;
	/* 0xC */ f32 tier1Accel;
	/* 0x10 */ f32 tier2Accel;
	/* 0x14 */ f32 tier3Accel;
	/* 0x18 */ f32 offroadSpeedCap;
	/* 0x1C */ s32 maxAir;
	/* 0x20 */ s32 airDrain;
	/* 0x24 */ s32 driftCost;
	/* 0x28 */ s32 boostCost;
	/* 0x2C */ s32 tornadoCost;
	/* 0x30 */ f32 driftDashSpeed;
	/* 0x34 */ f32 boostSpeed;

	void updateGearLevelStats(const GearLevelStats &stats){
		maxAir = stats.maxAir;
		airDrain = stats.passiveAirDrain;
		driftCost = stats.driftingAirCost;
		boostCost = stats.boostCost;
		tornadoCost = stats.tornadoCost;
		boostSpeed = stats.boostSpeed;
	}
};
static_assert(sizeof(PlayerGearStats) == 0x38);

struct Player_UnkStruct {
    s32 unk0;
    s32 unk4;
};

struct Character {
	Type type;
    u8 statPreset;
	BOOL mirroredModel;
    u8 model;
    u32 unkx4;
    f32 cameraHeight;
    f32 speedMultiplier;

    s8 lvl1Dash;
    s8 lvl2Dash;
    s8 lvl3Dash;
	s8 padding;

    s8 lvl1Limit;
    s8 lvl2Limit;
    s8 lvl3Limit;
	s8 padding1;

    s8 lvl1Power;
    s8 lvl2Power;
    s8 lvl3Power;
	s8 padding2;

    s8 lvl1Cornering;
    s8 lvl2Cornering;
    s8 lvl3Cornering;
	s8 padding3;
};
global std::array<Character, TotalCharacterAmount> Characters;

struct Gear {
    /* 0x0 */ u32 useFlags; // which characters can use it
    /* 0x4 */ u8 type;
    /* 0x5 */ u8 model;
    /* 0x6 */ u8 costInGearShop;
    /* 0x7 */ u8 unk7;
	/* 0x8 */ u32 unk8;
	/* 0xC */ f32 acceleration;
	/* 0x10 */ f32 topSpeed;
	/* 0x14 */ f32 offRoadSpeed;
	/* 0x18 */ f32 speedHandlingMultiplier;
	/* 0x1C */ f32 weight;
	/* 0x20 */ u32 extraTypeAttributes;
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
	/* 0x60 */ u32 specialFlags;
	/* 0x64 */ f32 jumpChargeCostMultiplier;
	/* 0x68 */ GearLevelStats levelStats[3];
	/* 0xBC */ s8 tempoStat;
	/* 0xBD */ s8 efficiencyStat;
	/* 0xBE */ s8 combatStat;
	/* 0xBF */ s8 weightStat;
	/* 0xC0 */ u32 unkC0;
	/* 0xC4 */ u32 unkC4;
	/* 0xC8 */ f32 exhaustTrailWidth;
	/* 0xCC */ f32 secondExhaustTrailWidth;
    char filler[0x100];
};
global std::array<Gear,TotalGearAmount> Gears;

struct Matrix4x4 {
    f32 field[16];
};

struct Matrix3x3 {
    f32 field[12];
};

struct RailQTE; // This is never defined, doesn't error only because its never dereferenced
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct Player {
	// 0x1080 in size
	/* 0x0 */ Controller *input;
	/* 0x4 */ u8 tornadoInvulnerabilityTimer;
	/* 0x5 */ u8 unused1;
	/* 0x6 */ u8 unused2;
	/* 0x7 */ u8 unused3;
	/* 0x8 */ BOOL ignoreTurbulence; // by Z button or Y toggle
	/* 0x9 */ u8 itemBox_cooldown;
	/* 0xA */ u8 unkA;
	/* 0xB */ u8 unkB;
	/* 0xC */ std::array<u16, 0x57> animationIDMap;
	/* 0xBA */ u8 character;
	/* 0xBB */ ExtremeGear extremeGear;
	/* 0xBC */ BOOL aiControl;
	/* 0xBD */ BOOL playerType;
	/* 0xBE */ GearType gearType;
	/* 0xBF */ u8 attributes;
	/* 0xC0 */ u32 unkC0;
	/* 0xC4 */ Matrix3x3 unkC4;
	/* 0xF4 */ u32 current_itemID;
	/* 0xF8 */ u8 tornadoIgnore_invincibilityTimer;
	/* 0xF9 */ char empty2[0x1]; // used to be the hang on infinite rings timer?
	// /* 0xF9 */ u8 unused4;
	// /* 0xFA */ u8 unused5;
	// /* 0xFB */ u8 unused6;
	/* 0xFC */ Collision *currentCollision;
	/* 0x100 */ u32 level4; // this is a bool
	/* 0x104 */ u8 maxJump;
	/* 0x105 */ u8 midJump;
	/* 0x106 */ u8 badJump;
	/* 0x107 */ u8 absoluteMaxJump;
	/* 0x108 */ u8 last_itemID_lap;
	/* 0x109 */ u8 trickCount;
	/* 0x10C */ u32 last_itemID;
	/* 0x110 */ BOOL last_itemBox_random; // was the last picked up item box random
	/* 0x111 */ BOOL last_level4; // if player has been level 4 before
	/* 0x112 */ BOOL rainbowTrailState; // almost always 1, used for init
	/* 0x113 */ u8 dreamTrail_timer;
	/* 0x114 */ u32 exhaustTrailColor;
	/* 0x118 */ u8 berserkerCooldown;
	/* 0x119 */ char empty[0x1];
	union {
		/* 0x11A */ u8 coverF_archetype;
		/* 0x11A */ u8 superFormState;
		/* 0x11A */ u8 coverP_weightState;
	};
	/* 0x11B */ BOOL slipstream;
	/* 0x11C */ f32 magneticImpulse_timer;
	/* 0x120 */ f32 turningSpeedLoss;
	/* 0x124 */ u32 unk124;
	/* 0x128 */ u32 unk128;
	/* 0x12C */ u32 unk12C;
	/* 0x130 */ u32 unk130;
	/* 0x134 */ u32 unk134;
	/* 0x138 */ u32 unk138;
	/* 0x13C */ u32 unk13C;
	/* 0x140 */ u32 unk140;
	/* 0x144 */ u32 unk144;
	/* 0x148 */ u32 unk148;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ u32 unk150;
	/* 0x154 */ u32 unk154;
	/* 0x158 */ u32 unk158;
	/* 0x15C */ u32 unk15C;
	/* 0x160 */ u32 unk160;
	/* 0x164 */ u32 unk164;
	/* 0x168 */ u32 unk168;
	/* 0x16C */ u32 unk16C;
	/* 0x170 */ u32 unk170;
	/* 0x174 */ u32 unk174;
	/* 0x178 */ u32 unk178;
	/* 0x17C */ u32 unk17C;
	/* 0x180 */ u32 unk180;
	/* 0x184 */ u32 unk184;
	/* 0x188 */ u32 unk188;
	/* 0x18C */ u32 unk18C;
	/* 0x190 */ u32 unk190;
	/* 0x194 */ u32 unk194;
	/* 0x198 */ u32 unk198;
	/* 0x19C */ u32 unk19C;
	/* 0x1A0 */ u32 unk1A0;
	/* 0x1A4 */ u32 unk1A4;
	/* 0x1A8 */ u32 unk1A8;
	/* 0x1AC */ u32 unk1AC;
	/* 0x1B0 */ u32 unk1B0;
	/* 0x1B4 */ u32 unk1B4;
	/* 0x1B8 */ u32 unk1B8;
	/* 0x1BC */ u32 unk1BC;
	/* 0x1C0 */ u32 unk1C0;
	/* 0x1C4 */ u32 unk1C4;
	/* 0x1C8 */ u32 unk1C8;
	/* 0x1CC */ u32 unk1CC;
	/* 0x1D0 */ u32 unk1D0;
	/* 0x1D4 */ u32 unk1D4;
	/* 0x1D8 */ u32 unk1D8;
	/* 0x1DC */ u32 unk1DC;
	/* 0x1E0 */ u32 unk1E0;
	/* 0x1E4 */ f32 x;
	/* 0x1E8 */ f32 y;
	/* 0x1EC */ f32 z;
	/* 0x1F0 */ f32 verticalRotation;
	/* 0x1F4 */ f32 horizontalRotation;
	/* 0x1F8 */ f32 rotationRoll;
	/* 0x1FC */ u32 unk1FC;
	/* 0x200 */ u32 unk200;
	/* 0x204 */ u32 unk204;
	/* 0x208 */ u32 unk208;
	/* 0x20C */ u32 unk20C;
	/* 0x210 */ u32 unk210;
	/* 0x214 */ u32 unk214;
	/* 0x218 */ u32 unk218;
	/* 0x21C */ u32 unk21C;
	/* 0x220 */ u32 unk220;
	/* 0x224 */ u32 unk224;
	/* 0x228 */ u32 unk228;
	/* 0x22C */ u32 unk22C;
	/* 0x230 */ u32 unk230;
	/* 0x234 */ u32 unk234;
	/* 0x238 */ u32 unk238;
	/* 0x23C */ u32 unk23C;
	/* 0x240 - 0x24C */ Player_UnkStruct firstUnk[2];
	/* 0x240 */ //u32 unk240;
	/* 0x244 */ //u32 unk244;
	/* 0x248 */ //u32 unk248;
	/* 0x24C */ //u32 unk24C;
	/* 0x250 */ u32 unk250;
	/* 0x254 */ u32 unk254;
	/* 0x258 */ u32 unk258;
	/* 0x25C */ u32 unk25C;
	/* 0x260 */ u32 unk260;
	/* 0x264 */ u32 unk264;
	/* 0x268 */ u32 unk268;
	/* 0x26C */ u32 unk26C;
	/* 0x270 */ u32 unk270;
	/* 0x274 */ u32 unk274;
	/* 0x278 */ u32 unk278;
	/* 0x27C */ u32 unk27C;
	/* 0x280 */ u32 unk280;
	/* 0x284 */ u32 unk284;
	/* 0x288 */ u32 unk288;
	/* 0x28C */ u32 unk28C;
	/* 0x290 */ u32 unk290;
	/* 0x294 */ u32 unk294;
	/* 0x298 */ u32 unk298;
	/* 0x29C */ u32 unk29C;
	/* 0x2A0 */ u32 unk2A0;
	/* 0x2A4 */ u32 unk2A4;
	/* 0x2A8 */ u32 unk2A8;
	/* 0x2AC */ u32 unk2AC;
	/* 0x2B0 */ u32 unk2B0;
	/* 0x2B4 - 0x2C0 */ Player_UnkStruct secondUnk[2];
	/* 0x2B4 */ //u32 unk2B4;
	/* 0x2B8 */ //u32 unk2B8;
	/* 0x2BC */ //u32 unk2BC;
	/* 0x2C0 */ //u32 unk2C0;
	/* 0x2C4 */ u32 unk2C4;
	/* 0x2C8 */ u32 unk2C8;
	/* 0x2CC */ u32 unk2CC;
	/* 0x2D0 */ u32 unk2D0;
	/* 0x2D4 */ u32 unk2D4;
	/* 0x2D8 */ u32 unk2D8;
	/* 0x2DC */ u32 unk2DC;
	/* 0x2E0 */ u32 unk2E0;
	/* 0x2E4 */ u32 unk2E4;
	/* 0x2E8 */ u32 unk2E8;
	/* 0x2EC */ u32 unk2EC;
	/* 0x2F0 */ u32 unk2F0;
	/* 0x2F4 */ u32 unk2F4;
	/* 0x2F8 */ u32 unk2F8;
	/* 0x2FC */ u32 unk2FC;
	/* 0x300 */ u32 unk300;
	/* 0x304 */ u32 unk304;
	/* 0x308 */ u32 unk308;
	/* 0x30C */ u32 unk30C;
	/* 0x310 */ u32 unk310;
	/* 0x314 */ u32 unk314;
	/* 0x318 */ u32 unk318;
	/* 0x31C */ u32 unk31C;
	/* 0x320 */ u32 unk320;
	/* 0x324 */ u32 unk324;
	/* 0x328 */ u32 unk328;
	/* 0x32C */ u32 unk32C;
	/* 0x330 */ u32 unk330;
	/* 0x334 */ u32 unk334;
	/* 0x338 */ u32 unk338;
	/* 0x33C */ u32 unk33C;
	/* 0x340 */ u32 unk340;
	/* 0x344 */ u32 unk344;
	/* 0x348 */ u32 unk348;
	/* 0x34C */ u32 unk34C;
	/* 0x350 */ u32 unk350;
	/* 0x354 */ u32 unk354;
	/* 0x358 */ u32 unk358;
	/* 0x35C */ u32 unk35C;
	/* 0x360 */ u32 unk360;
	/* 0x364 */ u32 unk364;
	/* 0x368 */ u32 unk368;
	/* 0x36C */ u32 unk36C;
	/* 0x370 */ u32 unk370;
	/* 0x374 */ u32 unk374;
	/* 0x378 */ u32 unk378;
	/* 0x37C */ u32 unk37C;
	/* 0x380 */ u32 unk380;
	/* 0x384 */ u32 unk384;
	/* 0x388 */ u32 unk388;
	/* 0x38C */ u32 unk38C;
	/* 0x390 */ u32 unk390;
	/* 0x394 */ u32 unk394;
	/* 0x398 */ u32 unk398;
	/* 0x39C */ u32 unk39C;
	/* 0x3A0 */ u32 unk3A0;
	/* 0x3A4 */ u32 unk3A4;
	/* 0x3A8 */ u32 unk3A8;
	/* 0x3AC */ u32 unk3AC;
	/* 0x3B0 */ u32 unk3B0;
	/* 0x3B4 */ u32 unk3B4;
	/* 0x3B8 */ u32 unk3B8;
	/* 0x3BC */ u32 unk3BC;
	/* 0x3C0 */ u32 unk3C0;
	/* 0x3C4 */ u32 unk3C4;
	/* 0x3C8 */ u32 unk3C8;
	/* 0x3CC */ u32 unk3CC;
	/* 0x3D0 */ u32 unk3D0;
	/* 0x3D4 */ u32 unk3D4;
	/* 0x3D8 */ u32 unk3D8;
	/* 0x3DC */ u32 unk3DC;
	/* 0x3E0 */ u32 unk3E0;
	/* 0x3E4 */ u32 unk3E4;
	/* 0x3E8 */ u32 unk3E8;
	/* 0x3EC */ u32 unk3EC;
	/* 0x3F0 */ u32 unk3F0;
	/* 0x3F4 */ u32 unk3F4;
	/* 0x3F8 */ u32 unk3F8;
	/* 0x3FC */ u32 unk3FC;
	/* 0x400 */ u32 unk400;
	/* 0x404 */ u32 unk404;
	/* 0x408 */ u32 unk408;
	/* 0x40C */ u32 unk40C;
	/* 0x410 */ u32 unk410;
	/* 0x414 */ u32 unk414;
	/* 0x418 */ u32 unk418;
	/* 0x41C */ u32 unk41C;
	/* 0x420 */ u32 unk420;
	/* 0x424 */ u32 unk424;
	/* 0x428 */ u32 unk428;
	/* 0x42C */ u32 unk42C;
	/* 0x430 */ u32 unk430;
	/* 0x434 */ u32 unk434;
	/* 0x438 */ u32 unk438;
	/* 0x43C */ u32 unk43C;
	/* 0x440 */ u32 unk440;
	/* 0x444 */ u32 unk444;
	/* 0x448 */ u32 unk448;
	/* 0x44C */ u32 unk44C;
	/* 0x450 */ u32 unk450;
	/* 0x454 */ u32 unk454;
	/* 0x458 */ u32 unk458;
	/* 0x45C */ u32 unk45C;
	/* 0x460 */ u32 unk460;
	/* 0x464 */ u32 unk464;
	/* 0x468 */ u32 unk468;
	/* 0x46C */ u32 unk46C;
	/* 0x470 */ u32 unk470;
	/* 0x474 */ u32 unk474;
	/* 0x478 */ u32 unk478;
	/* 0x47C */ u32 unk47C;
	/* 0x480 */ u32 unk480;
	/* 0x484 */ u32 unk484;
	/* 0x488 */ u32 unk488;
	/* 0x48C */ u32 unk48C;
	/* 0x490 */ u32 unk490;
	/* 0x494 */ u32 unk494;
	/* 0x498 */ u32 unk498;
	/* 0x49C */ u32 unk49C;
	/* 0x4A0 */ u32 unk4A0;
	/* 0x4A4 */ u32 unk4A4;
	/* 0x4A8 */ u32 unk4A8;
	/* 0x4AC */ u32 unk4AC;
	/* 0x4B0 */ u32 unk4B0;
	/* 0x4B4 */ u32 unk4B4;
	/* 0x4B8 */ u32 unk4B8;
	/* 0x4BC */ u32 unk4BC;
	/* 0x4C0 */ u32 unk4C0;
	/* 0x4C4 */ u32 unk4C4;
	/* 0x4C8 */ u32 unk4C8;
	/* 0x4CC */ u32 unk4CC;
	/* 0x4D0 */ u32 unk4D0;
	/* 0x4D4 */ u32 unk4D4;
	/* 0x4D8 */ u32 unk4D8;
	/* 0x4DC */ u32 unk4DC;
	/* 0x4E0 */ u32 unk4E0;
	/* 0x4E4 */ u32 unk4E4;
	/* 0x4E8 */ u32 unk4E8;
	/* 0x4EC */ u32 unk4EC;
	/* 0x4F0 */ u32 unk4F0;
	/* 0x4F4 */ u32 unk4F4;
	/* 0x4F8 */ u32 unk4F8;
	/* 0x4FC */ u32 unk4FC;
	/* 0x500 */ u32 unk500;
	/* 0x504 */ u32 unk504;
	/* 0x508 */ u32 unk508;
	/* 0x50C */ u32 unk50C;
	/* 0x510 */ u32 unk510;
	/* 0x514 */ u32 unk514;
	/* 0x518 */ u32 unk518;
	/* 0x51C */ u32 unk51C;
	/* 0x520 */ u32 unk520;
	/* 0x524 */ u32 unk524;
	/* 0x528 */ u32 unk528;
	/* 0x52C */ u32 unk52C;
	/* 0x530 */ u32 unk530;
	/* 0x534 */ u32 unk534;
	/* 0x538 */ u32 unk538;
	/* 0x53C */ u32 unk53C;
	/* 0x540 */ u32 unk540;
	/* 0x544 */ u32 unk544;
	/* 0x548 */ u32 unk548;
	/* 0x54C */ u32 unk54C;
	/* 0x550 */ u32 unk550;
	/* 0x554 */ u32 unk554;
	/* 0x558 */ u32 unk558;
	/* 0x55C */ u32 unk55C;
	/* 0x560 */ u32 unk560;
	/* 0x564 */ u32 unk564;
	/* 0x568 */ u32 unk568;
	/* 0x56C */ u32 unk56C;
	/* 0x570 */ u32 unk570;
	/* 0x574 */ u32 unk574;
	/* 0x578 */ u32 unk578;
	/* 0x57C */ u32 unk57C;
	/* 0x580 */ u32 unk580;
	/* 0x584 */ u32 unk584;
	/* 0x588 */ u32 unk588;
	/* 0x58C */ f32 unk58C;
	/* 0x590 */ f32 unk590;
	/* 0x594 */ u32 unk594;
	/* 0x598 */ u32 unk598;
	/* 0x59C */ u32 unk59C;
	/* 0x5A0 */ u32 unk5A0;
	/* 0x5A4 */ u32 unk5A4;
	/* 0x5A8 */ f32 unk5A8;
	/* 0x5AC */ f32 unk5AC;
	/* 0x5B0 */ u32 unk5B0;
	/* 0x5B4 */ u32 unk5B4;
	/* 0x5B8 */ u32 unk5B8;
	/* 0x5BC */ u32 unk5BC;
	/* 0x5C0 */ u32 unk5C0;
	/* 0x5C4 */ f32 unk5C4;
	/* 0x5C8 */ f32 unk5C8;
	/* 0x5CC */ u32 unk5CC;
	/* 0x5D0 */ u32 unk5D0;
	/* 0x5D4 */ u32 unk5D4;
	/* 0x5D8 */ u32 unk5D8;
	/* 0x5DC */ u32 unk5DC;
	/* 0x5E0 */ u32 unk5E0;
	/* 0x5E4 */ u32 unk5E4;
	/* 0x5E8 */ u32 unk5E8;
	/* 0x5EC */ u32 unk5EC;
	/* 0x5F0 */ u32 unk5F0;
	/* 0x5F4 */ u32 unk5F4;
	/* 0x5F8 */ u32 unk5F8;
	/* 0x5FC */ u32 unk5FC;
	/* 0x600 */ u32 unk600;
	/* 0x604 */ u32 unk604;
	/* 0x608 */ u32 unk608;
	/* 0x60C */ u32 unk60C;
	/* 0x610 */ u32 unk610;
	/* 0x614 */ u32 unk614;
	/* 0x618 */ u32 unk618;
	/* 0x61C */ RailQTE *rail_QTE_pointer;
	/* 0x620 */ u16 unk620;
	/* 0x622 */ u16 unk622;
	/* 0x624 */ u16 railState;
	/* 0x626 */ u16 railPart;
	/* 0x628 */ u16 railID;
	/* 0x62A */ u16 unk62A;
	/* 0x62C */ u32 unk62C;
	/* 0x630 */ u32 unk630;
	/* 0x634 */ u32 unk634;
	/* 0x638 */ u32 unk638;
	/* 0x63C */ u32 unk63C;
	/* 0x640 */ u32 unk640;
	/* 0x644 */ u32 unk644;
	/* 0x648 */ u32 unk648;
	/* 0x64C */ u32 unk64C;
	/* 0x650 */ u32 unk650;
	/* 0x654 */ u32 unk654;
	/* 0x658 */ u32 unk658;
	/* 0x65C */ u32 unk65C;
	/* 0x660 */ u32 unk660;
	/* 0x664 */ u32 unk664;
	/* 0x668 */ u32 unk668;
	/* 0x66C */ u32 unk66C;
	/* 0x670 */ u32 unk670;
	/* 0x674 */ u32 unk674;
	/* 0x678 */ u32 unk678;
	/* 0x67C */ u32 unk67C;
	/* 0x680 */ u32 unk680;
	/* 0x684 */ u32 unk684;
	/* 0x688 */ u32 unk688;
	/* 0x68C */ u32 unk68C;
	/* 0x690 */ u32 unk690;
	/* 0x694 */ u32 unk694;
	/* 0x698 */ u32 unk698;
	/* 0x69C */ u32 unk69C;
	/* 0x6A0 */ u32 unk6A0;
	/* 0x6A4 */ u32 unk6A4;
	/* 0x6A8 */ u32 unk6A8;
	/* 0x6AC */ u32 unk6AC;
	/* 0x6B0 */ u32 unk6B0;
	/* 0x6B4 */ u32 unk6B4;
	/* 0x6B8 */ u32 unk6B8;
	/* 0x6BC */ u32 unk6BC;
	/* 0x6C0 */ u16 collisionProperties;
	/* 0x6C2 */ u16 unk6C2;
	/* 0x6C4 */ u16 particleState;
	/* 0x6C6 */ u16 unk6C6;
	/* 0x6C8 */ u16 turbulenceState;
	/* 0x6CA */ u16 unk6CA;
	/* 0x6CC */ u32 unk6CC;
	/* 0x6D0 */ u32 unk6D0;
	/* 0x6D4 */ u32 unk6D4;
	/* 0x6D8 */ u32 unk6D8;
	/* 0x6DC */ u32 unk6DC;
	/* 0x6E0 */ u32 unk6E0;
	/* 0x6E4 */ u32 unk6E4;
	/* 0x6E8 */ u32 unk6E8;
	/* 0x6EC */ u32 unk6EC;
	/* 0x6F0 */ u32 unk6F0;
	/* 0x6F4 */ u32 unk6F4;
	/* 0x6F8 */ u32 unk6F8;
	/* 0x6FC */ u32 unk6FC;
	/* 0x700 */ u32 unk700;
	/* 0x704 */ u32 unk704;
	/* 0x708 */ u32 unk708;
	/* 0x70C */ u32 unk70C;
	/* 0x710 */ u32 unk710;
	/* 0x714 */ u32 unk714;
	/* 0x718 */ u32 unk718;
	/* 0x71C */ u32 unk71C;
	/* 0x720 */ u32 unk720;
	/* 0x724 */ u32 unk724;
	/* 0x728 */ u32 unk728;
	/* 0x72C */ u32 unk72C;
	/* 0x730 */ u32 unk730;
	/* 0x734 */ u32 unk734;
	/* 0x738 */ u32 unk738;
	/* 0x73C */ u32 unk73C;
	/* 0x740 */ u32 unk740;
	/* 0x744 */ u32 unk744;
	/* 0x748 */ u32 unk748;
	/* 0x74C */ u32 unk74C;
	/* 0x750 */ u32 unk750;
	/* 0x754 */ u32 unk754;
	/* 0x758 */ u32 unk758;
	/* 0x75C */ u32 unk75C;
	/* 0x760 */ u32 unk760;
	/* 0x764 */ u32 unk764;
	/* 0x768 */ u32 unk768;
	/* 0x76C */ u32 unk76C;
	/* 0x770 */ u32 unk770;
	/* 0x774 */ u32 unk774;
	/* 0x778 */ u32 unk778;
	/* 0x77C */ u32 unk77C;
	/* 0x780 */ u32 unk780;
	/* 0x784 */ u32 unk784;
	/* 0x788 */ u32 unk788;
	/* 0x78C */ u32 unk78C;
	/* 0x790 */ u32 unk790;
	/* 0x794 */ u32 unk794;
	/* 0x798 */ u32 unk798;
	/* 0x79C */ u32 unk79C;
	/* 0x7A0 */ u32 unk7A0;
	/* 0x7A4 */ u32 unk7A4;
	/* 0x7A8 */ u32 unk7A8;
	/* 0x7AC */ u32 unk7AC;
	/* 0x7B0 */ u32 unk7B0;
	/* 0x7B4 */ u32 unk7B4;
	/* 0x7B8 */ u32 unk7B8;
	/* 0x7BC */ u32 unk7BC;
	/* 0x7C0 */ u32 unk7C0;
	/* 0x7C4 */ u32 unk7C4;
	/* 0x7C8 */ u32 unk7C8;
	/* 0x7CC */ u32 unk7CC;
	/* 0x7D0 */ u32 unk7D0;
	/* 0x7D4 */ u32 unk7D4;
	/* 0x7D8 */ u32 unk7D8;
	/* 0x7DC */ u32 unk7DC;
	/* 0x7E0 */ u32 unk7E0;
	/* 0x7E4 */ u32 unk7E4;
	/* 0x7E8 */ u32 unk7E8;
	/* 0x7EC */ u32 unk7EC;
	/* 0x7F0 */ u32 unk7F0;
	/* 0x7F4 */ u32 unk7F4;
	/* 0x7F8 */ f32 unk7F8;
	/* 0x7FC */ u32 unk7FC;
	/* 0x800 */ u32 unk800;
	/* 0x804 */ u32 unk804;
	/* 0x808 */ u32 unk808;
	/* 0x80C */ u32 unk80C;
	/* 0x810 */ u32 unk810;
	/* 0x814 */ u32 unk814;
	/* 0x818 */ u32 unk818;
	/* 0x81C */ u32 unk81C;
	/* 0x820 */ u32 unk820;
	/* 0x824 */ u32 unk824;
	/* 0x828 */ u32 unk828;
	/* 0x82C */ u32 unk82C;
	/* 0x830 */ u32 unk830;
	/* 0x834 */ u32 unk834;
	/* 0x838 */ u32 unk838;
	/* 0x83C */ u32 unk83C;
	/* 0x840 */ u32 unk840;
	/* 0x844 */ u32 unk844;
	/* 0x848 */ u32 unk848;
	/* 0x84C */ u32 unk84C;
	/* 0x850 */ u32 unk850;
	/* 0x854 */ u32 unk854;
	/* 0x858 */ u32 unk858;
	/* 0x85C */ u32 unk85C;
	/* 0x860 */ u32 unk860;
	/* 0x864 */ u32 unk864;
	/* 0x868 */ u32 unk868;
	/* 0x86C */ u32 unk86C;
	/* 0x870 */ u32 unk870;
	/* 0x874 */ u32 unk874;
	/* 0x878 */ u32 unk878;
	union {
		/* 0x87C */ u32 fastest_timer;
		/* 0x87C */ u32 superTails_transformCooldown;
		/* 0x87C */ u32 hyperSonic_totalLinkTimer;
	};
	/* 0x880 */ u32 unk880;
	/* 0x884 */ u32 unk884;
	/* 0x888 */ u32 unk888;
	/* 0x88C */ u32 unk88C;
	/* 0x890 */ u32 unk890;
	/* 0x894 */ u32 unk894;
	/* 0x898 */ void* boostEnd;
	/* 0x89C */ u32 unk89C;
	/* 0x8A0 */ u32 unk8A0;
	/* 0x8A4 */ u32 unk8A4;
	/* 0x8A8 */ u32 unk8A8;
	/* 0x8AC */ s32 boostDuration;
	/* 0x8B0 */ u32 unk8B0;
	/* 0x8B4 */ u32 unk8B4;
	/* 0x8B8 */ u32 unk8B8;
	/* 0x8BC */ u32 unk8BC;
	/* 0x8C0 */ u32 lightBoard_flag;
	/* 0x8C4 */ u32 unk8C4;
	/* 0x8C8 */ u32 unk8C8;
	/* 0x8CC */ u32 unk8CC;
	/* 0x8D0 */ u32 unk8D0;
	/* 0x8D4 */ u32 unk8D4;
	/* 0x8D8 */ u32 unk8D8;
	/* 0x8DC */ PlayerGearStats gearStats[3]; // there are 3 of them for each level
	/* 0x914 */ //struct PlayerGearStats level2_gearStats;
	/* 0x94C */ //struct PlayerGearStats level3_gearStats;
	/* 0x984 */ s32 currentAir;
	/* 0x988 */ s32 changeInAir_gain;
	/* 0x98C */ s32 changeInAir_loss;
	/* 0x990 */ u32 unk990;
	/* 0x994 */ f32 weight;
	/* 0x998 */ u32 unk998;
	/* 0x99C */ u32 unk99C;
	/* 0x9A0 */ u32 unk9A0;
	/* 0x9A4 */ u32 unk9A4;
	/* 0x9A8 */ u32 unk9A8;
	/* 0x9AC */ u32 unk9AC;
	/* 0x9B0 */ u32 unk9B0;
	/* 0x9B4 */ u32 unk9B4;
	/* 0x9B8 */ u32 unk9B8;
	/* 0x9BC */ u32 unk9BC;
	/* 0x9C0 */ u32 unk9C0;
	/* 0x9C4 */ s32 requiredDriftDashFrames;
	/* 0x9C8 */ u32 unk9C8;
	/* 0x9CC */ s32 shortcutAirGainMultiplier;
	/* 0x9D0 */ u32 unk9D0;
	/* 0x9D4 */ SpecialFlags specialFlags;
	/* 0x9D8 */ u32 unk9D8;
	/* 0x9DC */ u32 unk9DC;
	/* 0x9E0 */ u32 trickSpeed;
	/* 0x9E4 */ u32 unk9E4;
	/* 0x9E8 */ u32 trickDirection;
	/* 0x9EC */ u32 unk9EC;
	/* 0x9F0 */ u32 unk9F0;
	/* 0x9F4 */ u16 jumpCharge;
	/* 0x9F6 */ u8 unk9F6;
	/* 0x9F7 */ u8 unk9F7;
	/* 0x9F8 */ u8 startingTrick;
	/* 0x9F9 */ u8 rampType;
	/* 0x9FA */ u8 unk9FA;
	/* 0x9FB */ u8 unk9FB;
	/* 0x9FC */ u8 inaccurate_trickCount; // counts accurately to trickCount 0x109
	/* 0x9FD */ u8 unk9FD;
	/* 0x9FE */ u8 trickTrajectory;
	/* 0x9FF */ TrickRanks trickRank;
	/* 0xA00 */ u8 unkA00;
	/* 0xA01 */ u8 unkA01;
	/* 0xA02 */ u8 unkA02;
	/* 0xA03 */ u8 trickFail; // whether you failed your trick
	/* 0xA04 */ u32 unkA04;
	/* 0xA08 */ u32 unkA08;
	/* 0xA0C */ u32 unkA0C;
	/* 0xA10 */ u32 unkA10;
	/* 0xA14 */ u32 unkA14;
	/* 0xA18 */ u32 unkA18;
	/* 0xA1C */ u32 unkA1C;
	/* 0xA20 */ u32 unkA20;
	/* 0xA24 */ u32 unkA24;
	/* 0xA28 */ MovementFlags movementFlags;
	/* 0xA2C */ MovementFlags last_movementFlags;
	/* 0xA30 */ u32 unkA30;
	/* 0xA34 */ u32 unkA34;
	/* 0xA38 */ u8 unknownVal;
	/* 0xA39 */ u8 unkA39;
	/* 0xA3A */ u8 unkA3A;
	/* 0xA3B */ u8 unkA3B;
	/* 0xA3C */ u32 unkA3C;
	/* 0xA40 */ Character *characterptr;
	/* 0xA44 */ Gear *gearptr;
	/* 0xA48 */ u32 unkA48;
	/* 0xA4C */ u32 unkA4C;
	/* 0xA50 */ u32 unkA50;
	/* 0xA54 */ u32 unkA54;
	/* 0xA58 */ u32 unkA58;
	/* 0xA5C */ u32 unkA5C;
	/* 0xA60 */ u32 unkA60;
	/* 0xA64 */ u32 unkA64;
	/* 0xA68 */ u32 unkA68;
	/* 0xA6C */ u32 unkA6C;
	/* 0xA70 */ u32 unkA70;
	/* 0xA74 */ u32 unkA74;
	/* 0xA78 */ u32 unkA78;
	/* 0xA7C */ u32 unkA7C;
	/* 0xA80 */ f32 gravity;
	/* 0xA84 */ u32 unkA84;
	/* 0xA88 */ u32 unkA88;
	/* 0xA8C */ u32 unkA8C;
	/* 0xA90 */ u32 unkA90;
	/* 0xA94 */ u32 unkA94;
	/* 0xA98 */ u32 unkA98;
	/* 0xA9C */ u32 unkA9C;
	/* 0xAA0 */ u32 unkAA0;
	/* 0xAA4 */ f32 verticalSpeed;
	/* 0xAA8 */ f32 generalSpeedLoss;
	/* 0xAAC */ f32 speed;
	/* 0xAB0 */ f32 unkAB0;
	/* 0xAB4 */ f32 speedCap;
	/* 0xAB8 */ f32 unkAB8;
	/* 0xABC */ u32 unkABC;
	/* 0xAC0 */ u32 unkAC0;
	/* 0xAC4 */ u32 unkAC4;
	/* 0xAC8 */ u32 unkAC8;
	/* 0xACC */ u32 unkACC;
	/* 0xAD0 */ u32 unkAD0;
	/* 0xAD4 */ f32 otherPlayerDistance[8];
	/* 0xAF4 */ u32 unkAF4;
	/* 0xAF8 */ u32 unkAF8;
	/* 0xAFC */ u32 unkAFC;
	/* 0xB00 */ u32 unkB00;
	/* 0xB04 */ u32 unkB04;
	/* 0xB08 */ u32 unkB08;
	/* 0xB0C */ u32 unkB0C;
	/* 0xB10 */ u32 unkB10;
	/* 0xB14 */ u32 unkB14;
	/* 0xB18 */ u32 unkB18;
	/* 0xB1C */ u32 unkB1C;
	/* 0xB20 */ u32 unkB20;
	/* 0xB24 */ f32 unkB24;
	/* 0xB28 */ f32 unkB28;
	/* 0xB2C */ f32 unkB2C;
	/* 0xB30 */ u32 unkB30;
	/* 0xB34 */ u32 unkB34;
	/* 0xB38 */ u32 unkB38;
	/* 0xB3C */ u32 unkB3C;
	/* 0xB40 */ u32 unkB40;
	/* 0xB44 */ u32 unkB44;
	/* 0xB48 */ u32 unkB48;
	/* 0xB4C */ u32 unkB4C;
	/* 0xB50 */ u32 unkB50;
	/* 0xB54 */ s32 driftDashFrames;
	/* 0xB58 */ u32 unkB58;
	/* 0xB5C */ u32 unkB5C;
	/* 0xB60 */ u32 unkB60;
	/* 0xB64 */ u32 unkB64;
	/* 0xB68 */ u32 unkB68;
	/* 0xB6C */ u32 unkB6C;
	/* 0xB70 */ u32 unkB70;
	/* 0xB74 */ u32 unkB74;
	/* 0xB78 */ u32 unkB78;
	/* 0xB7C */ u32 boostingAnimationID;
	/* 0xB80 */ u32 unkB80;
	/* 0xB84 */ u32 unkB84;
	/* 0xB88 */ u32 unkB88;
	/* 0xB8C */ u32 unkB8C;
	/* 0xB90 */ u32 unkB90;
	/* 0xB94 */ u32 unkB94;
	/* 0xB98 */ u32 rings;
	/* 0xB9C */ u32 unkB9C;
	/* 0xBA0 */ u32 unkBA0;
	/* 0xBA4 */ u32 unkBA4;
	/* 0xBA8 */ u32 flags;
	/* 0xBAC */ u32 unkBAC;
	/* 0xBB0 */ u32 statusEffectFlags;
	/* 0xBB4 */ u32 unkBB4;
	/* 0xBB8 */ u32 unkBB8;
	/* 0xBBC */ u32 unkBBC;
	/* 0xBC0 */ u32 unkBC0;
	/* 0xBC4 */ f32 stageProgress;
	/* 0xBC8 */ u32 unkBC8;
	/* 0xBCC */ u32 unkBCC;
	/* 0xBD0 */ u32 typeAttributes;
	/* 0xBD4 */ u32 unkBD4;
	/* 0xBD8 */ u32 unkBD8;
	/* 0xBDC */ u32 unkBDC;
	/* 0xBE0 */ u32 unkBE0;
	/* 0xBE4 */ u32 unkBE4;
	/* 0xBE8 */ u32 unkBE8;
	/* 0xBEC */ u32 unkBEC;
	/* 0xBF0 */ u32 unkBF0;
	/* 0xBF4 */ u32 unkBF4;
	/* 0xBF8 */ u32 unkBF8;
	/* 0xBFC */ u32 unkBFC;
	/* 0xC00 */ u32 unkC00;
	/* 0xC04 */ u32 unkC04;
	/* 0xC08 */ u32 unkC08;
	/* 0xC0C */ u32 unkC0C;
	/* 0xC10 */ u32 unkC10;
	/* 0xC14 */ u32 unkC14;
	/* 0xC18 */ u32 unkC18;
	/* 0xC1C */ u32 unkC1C;
	/* 0xC20 */ u32 unkC20;
	/* 0xC24 */ u32 unkC24;
	/* 0xC28 */ u32 unkC28;
	/* 0xC2C */ u32 unkC2C;
	/* 0xC30 */ u32 unkC30;
	/* 0xC34 */ u32 unkC34;
	/* 0xC38 */ u32 unkC38;
	/* 0xC3C */ u32 unkC3C;
	/* 0xC40 */ u32 unkC40;
	/* 0xC44 */ u32 unkC44;
	/* 0xC48 */ u32 unkC48;
	/* 0xC4C */ u32 unkC4C;
	/* 0xC50 */ u32 unkC50;
	/* 0xC54 */ u32 unkC54;
	/* 0xC58 */ u32 unkC58;
	/* 0xC5C */ u32 unkC5C;
	/* 0xC60 */ u32 unkC60;
	/* 0xC64 */ u32 unkC64;
	/* 0xC68 */ u32 unkC68;
	/* 0xC6C */ u32 unkC6C;
	/* 0xC70 */ u32 unkC70;
	/* 0xC74 */ u32 unkC74;
	/* 0xC78 */ u32 unkC78;
	/* 0xC7C */ u32 unkC7C;
	/* 0xC80 */ u32 unkC80;
	/* 0xC84 */ u32 unkC84;
	/* 0xC88 */ u32 unkC88;
	/* 0xC8C */ u32 unkC8C;
	/* 0xC90 */ u32 unkC90;
	/* 0xC94 */ u32 unkC94;
	/* 0xC98 */ u32 unkC98;
	/* 0xC9C */ u32 unkC9C;
	/* 0xCA0 */ u32 unkCA0;
	/* 0xCA4 */ u32 unkCA4;
	/* 0xCA8 */ u32 unkCA8;
	/* 0xCAC */ u32 unkCAC;
	/* 0xCB0 */ u32 unkCB0;
	/* 0xCB4 */ u32 unkCB4;
	/* 0xCB8 */ u32 unkCB8;
	/* 0xCBC */ u32 unkCBC;
	/* 0xCC0 */ u32 unkCC0;
	/* 0xCC4 */ u32 unkCC4;
	/* 0xCC8 */ u32 unkCC8;
	/* 0xCCC */ u32 unkCCC;
	/* 0xCD0 */ u32 unkCD0;
	/* 0xCD4 */ u32 unkCD4;
	/* 0xCD8 */ u32 unkCD8;
	/* 0xCDC */ u32 unkCDC;
	/* 0xCE0 */ u32 unkCE0;
	/* 0xCE4 */ u32 unkCE4;
	/* 0xCE8 */ u32 unkCE8;
	/* 0xCEC */ u32 unkCEC;
	/* 0xCF0 */ u32 unkCF0;
	/* 0xCF4 */ u32 unkCF4;
	/* 0xCF8 */ u32 unkCF8;
	/* 0xCFC */ u32 unkCFC;
	/* 0xD00 */ u32 unkD00;
	/* 0xD04 */ u32 unkD04;
	/* 0xD08 */ u32 unkD08;
	/* 0xD0C */ u32 unkD0C;
	/* 0xD10 */ u32 unkD10;
	/* 0xD14 */ u32 unkD14;
	/* 0xD18 */ u32 unkD18;
	/* 0xD1C */ u32 unkD1C;
	/* 0xD20 */ u32 unkD20;
	/* 0xD24 */ u32 unkD24;
	/* 0xD28 */ u32 unkD28;
	/* 0xD2C */ u32 unkD2C;
	/* 0xD30 */ u32 unkD30;
	/* 0xD34 */ u32 unkD34;
	/* 0xD38 */ u32 unkD38;
	/* 0xD3C */ u32 unkD3C;
	/* 0xD40 */ u32 unkD40;
	/* 0xD44 */ u32 unkD44;
	/* 0xD48 */ u32 unkD48;
	/* 0xD4C */ u32 unkD4C;
	/* 0xD50 */ u32 unkD50;
	/* 0xD54 */ u32 unkD54;
	/* 0xD58 */ u32 unkD58;
	/* 0xD5C */ u32 unkD5C;
	/* 0xD60 */ u32 unkD60;
	/* 0xD64 */ u32 unkD64;
	/* 0xD68 */ u32 unkD68;
	/* 0xD6C */ u32 unkD6C;
	/* 0xD70 */ u32 unkD70;
	/* 0xD74 */ u32 unkD74;
	/* 0xD78 */ u32 unkD78;
	/* 0xD7C */ u32 unkD7C;
	/* 0xD80 */ u32 unkD80;
	/* 0xD84 */ u32 objectLinkCount;
	/* 0xD88 */ u32 unkD88;
	/* 0xD8C */ s32 unkD8C;
	/* 0xD90 */ u32 unkD90;
	/* 0xD94 */ u32 unkD94;
	/* 0xD98 */ u32 unkD98;
	/* 0xD9C */ u8 unkD9C;
	/* 0xD9D */ u8 unkD9D;
	/* 0xD9E */ s16 unkD9E;
	/* 0xDA0 */ u32 unkDA0;
	/* 0xDA4 */ u32 unkDA4;
	/* 0xDA8 */ u32 unkDA8;
	/* 0xDAC */ u32 unkDAC;
	/* 0xDB0 */ u32 unkDB0;
	/* 0xDB4 */ u32 unkDB4;
	/* 0xDB8 */ u32 unkDB8;
	/* 0xDBC */ u32 unkDBC;
	/* 0xDC0 */ u32 unkDC0;
	/* 0xDC4 */ u32 unkDC4;
	/* 0xDC8 */ u32 unkDC8;
	/* 0xDCC */ u32 unkDCC;
	/* 0xDD0 */ u32 unkDD0;
	/* 0xDD4 */ u32 unkDD4;
	/* 0xDD8 */ u32 unkDD8;
	/* 0xDDC */ u32 unkDDC;
	/* 0xDE0 */ u32 unkDE0;
	/* 0xDE4 */ u32 unkDE4;
	/* 0xDE8 */ u32 unkDE8;
	/* 0xDEC */ u32 unkDEC;
	/* 0xDF0 */ u32 unkDF0;
	/* 0xDF4 */ u32 unkDF4;
	/* 0xDF8 */ u32 unkDF8;
	/* 0xDFC */ u32 unkDFC;
	/* 0xE00 */ u32 unkE00;
	/* 0xE04 */ u32 unkE04;
	/* 0xE08 */ u32 unkE08;
	/* 0xE0C */ u32 unkE0C;
	/* 0xE10 */ u32 unkE10;
	/* 0xE14 */ u32 unkE14;
	/* 0xE18 */ u32 unkE18;
	/* 0xE1C */ u32 unkE1C;
	/* 0xE20 */ u32 unkE20;
	/* 0xE24 */ u32 unkE24;
	/* 0xE28 */ u32 unkE28;
	/* 0xE2C */ u32 unkE2C;
	/* 0xE30 */ u32 unkE30;
	/* 0xE34 */ u32 unkE34;
	/* 0xE38 */ u32 unkE38;
	/* 0xE3C */ u32 unkE3C;
	/* 0xE40 */ u32 unkE40;
	/* 0xE44 */ u32 unkE44;
	/* 0xE48 */ u32 unkE48;
	/* 0xE4C */ u32 unkE4C;
	/* 0xE50 */ u32 unkE50;
	/* 0xE54 */ u32 unkE54;
	/* 0xE58 */ u32 unkE58;
	/* 0xE5C */ u32 unkE5C;
	/* 0xE60 */ u32 unkE60;
	/* 0xE64 */ u32 unkE64;
	/* 0xE68 */ u32 unkE68;
	/* 0xE6C */ u32 unkE6C;
	/* 0xE70 */ u32 unkE70;
	/* 0xE74 */ u32 unkE74;
	/* 0xE78 */ u32 unkE78;
	/* 0xE7C */ u32 unkE7C;
	/* 0xE80 */ u32 unkE80;
	/* 0xE84 */ u32 unkE84;
	/* 0xE88 */ u32 unkE88;
	/* 0xE8C */ u32 unkE8C;
	/* 0xE90 */ u32 closestTurbulenceIndex;
	/* 0xE94 */ u32 unkE94;
	/* 0xE98 */ u32 unkE98;
	/* 0xE9C */ u32 unkE9C;
	/* 0xEA0 */ u32 unkEA0;
	/* 0xEA4 */ u32 unkEA4;
	/* 0xEA8 */ u32 unkEA8;
	/* 0xEAC */ u32 unkEAC;
	/* 0xEB0 */ u32 unkEB0;
	/* 0xEB4 */ u32 unkEB4;
	/* 0xEB8 */ u32 unkEB8;
	/* 0xEBC */ u32 unkEBC;
	/* 0xEC0 */ u32 unkEC0;
	/* 0xEC4 */ u32 unkEC4;
	/* 0xEC8 */ u32 unkEC8;
	/* 0xECC */ u32 unkECC;
	/* 0xED0 */ u32 unkED0;
	/* 0xED4 */ u32 unkED4;
	/* 0xED8 */ u32 unkED8;
	/* 0xEDC */ u32 unkEDC;
	/* 0xEE0 */ u32 unkEE0;
	/* 0xEE4 */ u32 unkEE4;
	/* 0xEE8 */ u32 unkEE8;
	/* 0xEEC */ u32 unkEEC;
	/* 0xEF0 */ u32 unkEF0;
	/* 0xEF4 */ u32 unkEF4;
	/* 0xEF8 */ u32 unkEF8;
	/* 0xEFC */ u32 unkEFC;
	/* 0xF00 */ u32 unkF00;
	/* 0xF04 */ u32 unkF04;
	/* 0xF08 */ u32 unkF08;
	/* 0xF0C */ u32 unkF0C;
	/* 0xF10 */ u32 unkF10;
	/* 0xF14 */ u32 unkF14;
	/* 0xF18 */ u32 unkF18;
	/* 0xF1C */ u32 unkF1C;
	/* 0xF20 */ u32 unkF20;
	/* 0xF24 */ u32 unkF24;
	/* 0xF28 */ u32 unkF28;
	/* 0xF2C */ u32 unkF2C;
	/* 0xF30 */ u32 unkF30;
	/* 0xF34 */ u32 unkF34;
	/* 0xF38 */ Player *attackedPlayer; // has pointer to the player who you're being attacked by
	/* 0xF3C */ Player *attackingPlayer; // has pointer to the player you're attacking
	/* 0xF40 */ u32 unkF40;
	/* 0xF44 */ u32 unkF44;
	/* 0xF48 */ u32 unkF48;
	/* 0xF4C */ u32 unkF4C;
	/* 0xF50 */ u32 unkF50;
	/* 0xF54 */ u32 unkF54;
	/* 0xF58 */ u32 unkF58;
	/* 0xF5C */ u32 unkF5C;
	/* 0xF60 */ u32 unkF60;
	/* 0xF64 */ u32 unkF64;
	/* 0xF68 */ u32 unkF68;
	/* 0xF6C */ u32 unkF6C;
	/* 0xF70 */ u32 unkF70;
	/* 0xF74 */ u32 unkF74;
	/* 0xF78 */ u32 unkF78;
	/* 0xF7C */ u32 unkF7C;
	/* 0xF80 */ u32 unkF80;
	/* 0xF84 */ u32 unkF84;
	/* 0xF88 */ u32 unkF88;
	/* 0xF8C */ u32 unkF8C;
	/* 0xF90 */ u32 unkF90;
	/* 0xF94 */ u32 unkF94;
	/* 0xF98 */ u32 unkF98;
	/* 0xF9C */ u32 unkF9C;
	/* 0xFA0 */ u32 unkFA0;
	/* 0xFA4 */ u32 unkFA4;
	/* 0xFA8 */ u32 unkFA8;
	/* 0xFAC */ u32 unkFAC;
	/* 0xFB0 */ u32 unkFB0;
	/* 0xFB4 */ u32 unkFB4;
	/* 0xFB8 */ u32 unkFB8;
	/* 0xFBC */ u32 unkFBC;
	/* 0xFC0 */ u32 unkFC0;
	/* 0xFC4 */ u32 unkFC4;
	/* 0xFC8 */ u32 unkFC8;
	/* 0xFCC */ u32 unkFCC;
	/* 0xFD0 */ u32 unkFD0;
	/* 0xFD4 */ u32 unkFD4;
	/* 0xFD8 */ u32 unkFD8;
	/* 0xFDC */ u32 unkFDC;
	/* 0xFE0 */ u32 unkFE0;
	/* 0xFE4 */ u32 unkFE4;
	/* 0xFE8 */ u32 unkFE8;
	/* 0xFEC */ u32 unkFEC;
	/* 0xFF0 */ u32 unkFF0;
	/* 0xFF4 */ u32 unkFF4;
	/* 0xFF8 */ u32 unkFF8;
	/* 0xFFC */ u32 unkFFC;
	/* 0x1000 */ u8 unk1000;
	/* 0x1001 */ u8 unk1001;
	/* 0x1002 */ u8 unk1002;
	/* 0x1003 */ u8 unk1003;
	/* 0x1004 */ u8 unk1004;
	/* 0x1005 */ u8 unk1005;
	/* 0x1006 */ u8 unk1006;
	/* 0x1007 */ u8 unk1007;
	/* 0x1008 */ u8 unk1008;
	/* 0x1009 */ u8 unk1009;
	/* 0x100A */ u8 unk100A;
	/* 0x100B */ u8 unk100B;
	/* 0x100C */ u8 unk100C;
	/* 0x100D */ u8 unk100D;
	/* 0x100E */ u8 unk100E;
	/* 0x100F */ u8 unk100F;
	/* 0x1010 */ u8 unk1010;
	/* 0x1011 */ u8 unk1011;
	/* 0x1012 */ u8 unk1012;
	/* 0x1013 */ u8 unk1013;
	/* 0x1014 */ u8 unk1014;
	/* 0x1015 */ u8 unk1015;
	/* 0x1016 */ u8 unk1016;
	/* 0x1017 */ u8 unk1017;
	/* 0x1018 */ u8 unk1018;
	/* 0x1019 */ u8 unk1019;
	/* 0x101A */ u16 reciproExtendTimer;
	/* 0x101C */ u8 unk101C;
	/* 0x101D */ u8 unk101D;
	/* 0x101E */ u16 death_spawnPoint;
	/* 0x1020 */ u8 unk1020;
	/* 0x1021 */ u8 unk1021;
	/* 0x1022 */ u8 unk1022;
	/* 0x1023 */ u8 unk1023;
	/* 0x1024 */ u8 unk1024;
	/* 0x1025 */ u8 unk1025;
	/* 0x1026 */ u8 unk1026;
	/* 0x1027 */ u8 unk1027;
	/* 0x1028 */ u8 unk1028;
	/* 0x1029 */ u8 index;
	/* 0x102A */ u8 currentLap;
	/* 0x102B */ u8 previousLap;
	/* 0x102C */ u8 placement_counter;
	/* 0x102D */ u8 placement;
	/* 0x102E */ u8 level;
	/* 0x102F */ u8 subState;
	/* 0x1030 */ u8 unk1030;
	/* 0x1031 */ u8 unk1031;
	/* 0x1032 */ u8 unk1032;
	/* 0x1033 */ u8 unk1033;
	/* 0x1034 */ PlayerState state;
	/* 0x1035 */ PlayerState previousState;
	/* 0x1036 */ u8 unk1036;
	/* 0x1037 */ u8 unk1037;
	/* 0x1038 */ u8 unk1038;
	/* 0x1039 */ u8 unk1039;
	/* 0x103A */ u8 unk103A;
	/* 0x103B */ u8 unk103B;
	/* 0x103C */ u8 unk103C;
	/* 0x103D */ u8 unk103D;
	/* 0x103E */ u8 unk103E;
	/* 0x103F */ u8 unk103F;
	/* 0x1040 */ u8 unk1040;
	/* 0x1041 */ u8 unk1041;
	/* 0x1042 */ u8 turbulenceTrickType;
	/* 0x1043 */ u8 qteState;
	/* 0x1044 */ u8 unk1044;
	/* 0x1045 */ u8 unk1045;
	/* 0x1046 */ u8 unk1046;
	/* 0x1047 */ u8 unk1047;
	/* 0x1048 */ u8 stage_subState;
	/* 0x1049 */ u8 unk1049;
	/* 0x104A */ u8 unk104A;
	/* 0x104B */ u8 unk104B;
	/* 0x104C */ u8 unk104C;
	/* 0x104D */ u8 unk104D;
	/* 0x104E */ u8 unk104E;
	/* 0x104F */ u8 unk104F;
	/* 0x1050 */ u8 unk1050;
	/* 0x1051 */ u8 unk1051;
	/* 0x1052 */ u8 unk1052;
	/* 0x1053 */ u8 unk1053;
	/* 0x1054 */ u8 unk1054;
	/* 0x1055 */ u8 unk1055;
	/* 0x1056 */ u8 unk1056;
	/* 0x1057 */ u8 unk1057;
	/* 0x1058 */ u8 unk1058;
	/* 0x1059 */ u8 unk1059;
	/* 0x105A */ u8 unk105A;
	/* 0x105B */ u8 unk105B;
	/* 0x105C */ u8 greenCave_subState;
	/* 0x105D */ u8 unk105D;
	/* 0x105E */ u8 unk105E;
	/* 0x105F */ u8 unk105F;
	/* 0x1060 */ u8 unk1060;
	/* 0x1061 */ u8 unk1061;
	/* 0x1062 */ u8 unk1062;
	/* 0x1063 */ u8 unk1063;
	/* 0x1064 */ u8 unk1064;
	/* 0x1065 */ u8 unk1065;
	/* 0x1066 */ u8 unk1066;
	/* 0x1067 */ u8 unk1067;
	/* 0x1068 */ u8 y_toggle;
	/* 0x1069 */ u8 hyperSonic_status;
	/* 0x106A */ u8 grindRailDash;
	/* 0x106B */ u8 flyHoopDash;
	/* 0x106C */ BOOL splashCanyonFlyRoute;
	/* 0x106D */ u8 unk106D;
	/* 0x106E */ u8 unk106E;
	/* 0x106F */ u8 unk106F;
	/* 0x1070 */ u8 unk1070;
	/* 0x1071 */ u8 unk1071;
	/* 0x1072 */ u8 unk1072;
	/* 0x1073 */ u8 unk1073;
	/* 0x1074 */ u8 unk1074;
	/* 0x1075 */ u8 unk1075;
	/* 0x1076 */ u8 unk1076;
	/* 0x1077 */ u8 unk1077;
	/* 0x1078 */ u8 hangOn_infiniteRingTimer;
	/* 0x1079 */ u8 magneticImpulse_soundStatus;
	/* 0x107A */ u8 unk107A;
	/* 0x107B */ u8 unk107B;
	/* 0x107C */ u8 characterArchetype;
	union {
		/* 0x107D */ BOOL hovercraft_alternatingRingPickup;
		/* 0x107D */ BOOL isBlastGaugeGear;
	};
	union {
		/* 0x107E */ BOOL fastest_superCruise;
		/* 0x107E */ BOOL stardustspeederII_1frameboost;
	};
	/* 0x107F */ u8 typeAmount;
};
static_assert(sizeof(Player) == 0x1080); // NOLINT(readability-magic-numbers)
#pragma GCC diagnostic pop

constexpr auto MaxPlayerCount = 8;
global std::array<Player, MaxPlayerCount> players;

struct PlayerCameraStruct {
	char filler[0x24A];
	u8 cameraPreset; // usually 0xFF, tries to catch a camera preset other than 0xFF
	u8 cameraPresetProperty; // some extra property to the preset
	char filler2[0x10];
};

global std::array<PlayerCameraStruct, MaxPlayerCount> playerCameraStruct;

struct Object1 {
	u32 x; // floats
	u32 y;
	u32 z;
	char unknown[0xC];
	s32 x_rotation;
	s32 y_rotation;
	s32 z_rotation;
	char filler[0x24];
	u32 item_icon;
};

struct ParticleTaskObject1 {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ void* unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ void* unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ void* unk68;
	/* 0x6C */ void* unk6C;
	/* 0x70 */ u8 unk70;
	/* 0x71 */ u8 unk71;
	/* 0x72 */ u8 unk72;
	/* 0x73 */ u8 unk73;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
};
static_assert(sizeof(ParticleTaskObject1) == 0x80); // NOLINT(readability-magic-numbers)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct Object {
	Task update;
	Object *back;
	Object *next;
	u16 flags;
	u16 object_group;
	void* object;
	u8 state;
	u16 object_type;
	u8 culling_group;
	u16 item_id;
	u16 object_id;
	u16 index;
};
#pragma GCC diagnostic pop

struct ObjectInfo {
	u16 objectType;
	u8 maxPlayerCount; // maximum amount of required players in the game for object to spawn
	u8 objectPortal;
	u32 spawnableGamemodes;
	union {
		u32 extraAttribute;
		u32 itemID; // for item boxes
		u32 controlLockFrames; // for fly hoops
	};
	f32 x;
	f32 y;
	f32 z;
	// BAMS rotations? multiplies this value with 182.044
	f32 xRotation;
	f32 yRotation;
	f32 zRotation;
	union {
		f32 extraAttribute2;
		f32 flyHoopSpeed; // don't use pSpeed for this
	};
	union {
		f32 extraAttribute3;
		f32 flyHoopScale;
	};
	f32 unknown;
};

struct HUDStruct {
	u16 screenX;
	u16 screenY;
	u32 textureX;
	u32 textureY;
	u32 textureXLength;
	u32 textureYLength;
	f32 scale;
};

struct Quaternion {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
	Quaternion() = default;
};

struct DATFile {
    char file[0x10];
    u32 spec1; // offset 0x10 in DAT file
    u32 spec2; // offset 0x18 in DAT file
    u32 *soundFileID; // pointer to IDs that play sounds like 0xA9300100
};

inline BOOL isSuperCharacter(const Player &player, const BOOL &excludeSuperSonic = false){
	const BOOL &usingChaosEmerald = player.extremeGear == ChaosEmerald;
	if(excludeSuperSonic) { return usingChaosEmerald; }
	return usingChaosEmerald || player.character == SuperSonic;
}

inline BOOL isSuperCharacter(const Player &player, const CharacterEnums &character){
	return player.character == character && isSuperCharacter(player);
}

global u32 gu32GameCnt; // current game time in frames
global u32 gu32Game_PauseFlag;

global u32 CurrentGameMode;
global u32 CurrentStage;
global u32 InGamePlayerCount;

global s32 RuleSettings;

global u16 CurrentObjectAmount;
global u32 MaxObjectAmount;
global Object *StartOfObjectsPointer;

[[nodiscard]] inline std::span<Object> getObjectList(){
        return {StartOfObjectsPointer, CurrentObjectAmount};
}

global char* DumpPackManFile(const char [], u32);
global void SetArchiveBinary(const char [], u32, u32);
global void SetupGVRTextureArchive(void *packmanStart, u32 *offsetFromStart, void *saveptr, u32 num);
global void PackMan_SetupRenderedText(void *packmanStart, u32 *offsetFromStart, void *saveptr);
global void LoadCustomDATFile(void*);

global Object* SetTask(Task, u32, u32);