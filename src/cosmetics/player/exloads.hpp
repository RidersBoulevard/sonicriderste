/** \file exloads.hpp
 * Header file for dealing with EX loads.
 */

#pragma once

#include <optional>
#include <span>
#include <string>
#include <riders/character.hpp>
#include <riders/gear.hpp>

#include "containers/graphicalobject.hpp"
#include "containers/rgba.hpp"

constexpr auto CharacterEXLoadSlots = 12;
constexpr auto GearEXLoadSlots = 10;

constexpr auto NotLockedGearSelectionID = 0x1A;
constexpr auto LockedGearSelectionID = 0x1B;

/**
 * Stores all of the EX load IDs for every unique EX load in the game.
 */
enum class EXLoad : u8 {
    None,///< This ID is used if no EX load is selected.
    PerfectNazo,
    E10R,
    DarkSonic,
    HyperSonic,
    StardustSpeeder,	// SYB: Probably a good idea to start recycling these unnecessary Gear
    HyperHangOn,		// entries after 2.4 considering the custom Gear system is in the code now
    TheProfessional,	// (Easier than removing them considering ASM checks for Miku etc).
    OllieKingGear,
    WindmasterJet,
    NeoMetal,
    E99,
    TheBeast,
    E10B,
    HatsuneMiku,
    MetalSonicNoScarf,
    Reala,
    GonGon,
    WCJet,
    WCStorm,
    WCWave,
    MCSonic,
    MCTails,
    MCKnuckles,
    SuperStorm,
    Pyro,
    HangOnAT,
    Selkadoom,
    SuperSonic,
    Throttle,
	Jackle,
	E10Y,
    // add ex loads before the Count enum please kthx
    Count
};
constexpr auto EXLoadCount = static_cast<size_t>(EXLoad::Count);

ASMDefined void lbl_0013FA84(u32 soundID); /* asm, some different version of PlaySoundFromDAT function */

struct EXLoadBoostColor {
    RGBANormalized rgbaFirstLayer; // first layer is the primary one, you see this one 90% of the time
    RGBANormalized rgbaSecondLayer; // this second layer starts flashing right as your boost is about to end
};

struct EnabledEXLoads {
    EXLoad characterExLoadID = EXLoad::None;
    EXLoad gearExLoadID = EXLoad::None;

    [[nodiscard]] constexpr bool isExloadEnabled() const {
        return characterExLoadID != EXLoad::None || gearExLoadID != EXLoad::None;
    }
};

enum class CharacterType : s8 {
    DefaultTypeCharacter = -1,
    SpeedCharacter,
    FlyCharacter,
    PowerCharacter,
    CPUCharacter,
};

// Cosmetic = for ex loads that are accessed via the L button (cosmetic ex loads)
// GameplayAltering = for ex loads that are accessed via the X button (gameplay altering ex loads)
// AllTypes... = used for scrolling through both L and X loads
enum class EXLoadTypes : u8 { //TODO: used to be u32, check for ASM bugs
	None,
	Cosmetic,
	GameplayAltering,
	CosmeticForwards,
	CosmeticBackwards,
	GameplayAlteringForwards,
	GameplayAlteringBackwards,
	AllTypes
};

constexpr std::array<f32, EXLoadCount> EXLoadWeights = {
	-1.0F,
	-1.0F,
	-1.0F,
	-1.0F,
	1.2F, // Hyper Sonic
	-1.0F,
	-1.0F,
	-1.0F,
	-1.0F,
	-1.0F,
	-1.0F,
	-1.0F,
	-1.0F,
	-1.0F,
	0.95F,// hatsune miku
	-1.0F,
	1.1F,// reala
	1.4F,// gongon
	1.0F,
	1.45F,
	0.95f,
	1.1f,
	0.9f,
	1.4f,
	-1.0f,
	-1.0f,
	-1.0f,
	-1.0f, // seelkadoom
	-1.0f, // super sonic
	-1.0f, // Throttle
	-1.0f, // jackle
	1.25f // E10Y
};

constexpr std::array<CharacterArchetype, EXLoadCount> EXLoadArchetypes = {
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::Combat,// hatsune miku
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::Boost,// reala
	CharacterArchetype::AllRounder,    // gongon
	CharacterArchetype::Windcatcher,
	CharacterArchetype::Windcatcher,
	CharacterArchetype::Windcatcher,
	CharacterArchetype::Mechanic,
	CharacterArchetype::Mechanic,
	CharacterArchetype::Mechanic,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad,
	CharacterArchetype::NoEXLoad, // seelkadoom
	CharacterArchetype::SuperSonic,// super sonic exload
	CharacterArchetype::NoEXLoad, // Throttle
	CharacterArchetype::NoEXLoad, // Jackle
	CharacterArchetype::NoEXLoad // E10Y
};

ASMUsed const std::array<RGBA32, EXLoadCount> EXLoadHUDColors;
ASMUsed const std::array<EXLoadBoostColor, EXLoadCount> EXLoadBoostColors;

struct EXLoadColors {
	const void *colors = nullptr;
	u32 index = 0;
};

struct EXLoadInfo {
	EXLoadTypes exLoadType = EXLoadTypes::None;	///< Whether is a L load or a X load, or no ex load
    EXLoad exLoadID = EXLoad::None;	///< taken from enum ::EXLoad
    Character::Enum character = Character::Invalid;	///< character it applies to, if none, it'll be -1 (0xFF)
    ExtremeGear::Enum gear = ExtremeGear::Invalid;	///< gear it applies to, if none, it'll be -1 (0xFF)
    u16 portraitTextureID = static_cast<u16>(-1);	///< what texture ID it will use for the portrait of the ex load
    u16 gearTextTextureID = static_cast<u16>(-1);	///< what texture ID it will use for gear text if it's a gear ex load. if it's a character ex load, this is used for their default gear specifically. otherwise -1
    u16 characterTextTextureID = static_cast<u16>(-1);	///< what texture ID it will use for character text if it's a character ex load, otherwise -1
    u16 defaultGearPortraitTextureID = static_cast<u16>(-1);	///< what texture ID it will use for character ex loads' default gears if applicable, otherwise -1
    CharacterType characterType = CharacterType::DefaultTypeCharacter;	///< if character ex load, what type they're supposed to be, -1 for default

	[[nodiscard]] constexpr auto id() const{
		return std::to_underlying(exLoadID);
	}

	[[nodiscard]] constexpr const auto& hudColor() const{
		return EXLoadHUDColors[id()];
	}

	[[nodiscard]] constexpr EXLoadColors exloadColor() const{
		return {EXLoadHUDColors.data(), id()};
	}

	[[nodiscard]] constexpr const auto& boostColor() const{
		return EXLoadBoostColors[id()];
	}

	[[nodiscard]] constexpr EXLoadColors exloadBoostColor() const{
		return {EXLoadBoostColors.data(), id()};
	}

	[[nodiscard]] constexpr const auto& weight() const{
		return EXLoadWeights[id()];
	}

	[[nodiscard]] constexpr const auto& archetype() const{
		return EXLoadArchetypes[id()];
	}
};

constexpr std::array<EXLoadInfo, CharacterEXLoadSlots> CharacterEXLoadDataSlots = {
        {
                {},
                {
                        .exLoadType = EXLoadTypes::GameplayAltering,
                        .exLoadID = EXLoad::E10R,
                        .character = Character::E10G,
                        .portraitTextureID = 0xEE,
                        .characterTextTextureID = 0x103,
                        .characterType = CharacterType::PowerCharacter,
                },
                {
						.exLoadType = EXLoadTypes::Cosmetic,
						.exLoadID = EXLoad::NeoMetal,
						.character = Character::MetalSonic,
						.portraitTextureID = 0xEF,
                },
                {
						.exLoadType = EXLoadTypes::GameplayAltering,
						.exLoadID = EXLoad::E10B,
						.character = Character::E10G,
                        .portraitTextureID = 0xBE,
						.gearTextTextureID = 0x23,
                        .characterTextTextureID = 0x56,
						.defaultGearPortraitTextureID = 0xA9,
                        .characterType = CharacterType::SpeedCharacter
                },
                {
						.exLoadType = EXLoadTypes::GameplayAltering,
						.exLoadID = EXLoad::HatsuneMiku,
						.character = Character::Ulala,
						.portraitTextureID = 0xF4,
						.gearTextTextureID = 0xFD,
						.characterTextTextureID = 0xFE,
						.defaultGearPortraitTextureID = 0x101,
                },
                {
						.exLoadType = EXLoadTypes::Cosmetic,
						.exLoadID = EXLoad::MetalSonicNoScarf,
						.character = Character::MetalSonic,
						.portraitTextureID = 0xF7,
                },
                {
						.exLoadType = EXLoadTypes::GameplayAltering,
						.exLoadID = EXLoad::Reala,
						.character = Character::Nights,
						.portraitTextureID = 0xF3,
						.gearTextTextureID = 0xFC,
						.characterTextTextureID = 0x100,
						.defaultGearPortraitTextureID = 0x102,
                },
                {
						.exLoadType = EXLoadTypes::GameplayAltering,
						.exLoadID = EXLoad::GonGon,
						.character = Character::Aiai,
						.portraitTextureID = 0xF2,
						.characterTextTextureID = 0xFF,
                },
                {
						.exLoadType = EXLoadTypes::Cosmetic,
						.exLoadID = EXLoad::Selkadoom,
						.character = Character::Shadow,
						.portraitTextureID = 0x107,
                },
                {
						.exLoadType = EXLoadTypes::GameplayAltering,
						.exLoadID = EXLoad::SuperSonic,
						.character = Character::Sonic,
						.portraitTextureID = 0xA0,
                },
				{
						.exLoadType = EXLoadTypes::Cosmetic,
						.exLoadID = EXLoad::Jackle,
						.character = Character::Nights,
						.portraitTextureID = 0x10B,
						.gearTextTextureID = 0xFC,
                        .characterTextTextureID = 0x10D,
						.defaultGearPortraitTextureID = 0x102,
                },
				{
						.exLoadType = EXLoadTypes::GameplayAltering,
						.exLoadID = EXLoad::E10Y,
						.character = Character::E10G,
						.portraitTextureID = 0x10A,
						.gearTextTextureID = 0x10E,
						.characterTextTextureID = 0x10C,
						.defaultGearPortraitTextureID = 0xA9,
						.characterType = CharacterType::FlyCharacter,
                },
        }
};

constexpr std::array<EXLoadInfo, GearEXLoadSlots> GearEXLoadDataSlots = {
        {
                {},
                {
						.exLoadType = EXLoadTypes::Cosmetic,
						.exLoadID = EXLoad::PerfectNazo,
						.character = Character::Sonic,
                        .gear = ExtremeGear::ChaosEmerald,
						.portraitTextureID = 0xED,
						.gearTextTextureID = 0x1A,
                },
                {
						.exLoadType = EXLoadTypes::Cosmetic,
						.exLoadID = EXLoad::DarkSonic,
						.character = Character::Sonic,
						.gear = ExtremeGear::ChaosEmerald,
						.portraitTextureID = 0xF0,
						.gearTextTextureID = 0x1A,
                },
                {
						.exLoadType = EXLoadTypes::GameplayAltering,
						.exLoadID = EXLoad::HyperSonic,
						.character = Character::Sonic,
						.gear = ExtremeGear::ChaosEmerald,
                        .portraitTextureID = 0xF1,
						.gearTextTextureID = 0x1A,
                },
        }
};

/**
 * Contains all required information and other miscellaneous things for the current EX load the Player has selected, if applicable.
 * Refer to #Player_EXLoadData for the variable that holds this information at runtime.
 */
class CurrentEXLoadInfo {
	using charExloadPtr = const EXLoadInfo*;
	using gearExloadPtr = const EXLoadInfo*;

public:
	static constexpr auto MaxDelayTime = 15;

	charExloadPtr characterExload = nullptr;
	gearExloadPtr gearExload = nullptr;
	u8 delayTime = 0;///< how many frames of delay before next input is read again, counts down to 0
};

extern std::array<CurrentEXLoadInfo, MaxPlayerCount> Player_EXLoadData;

/**
 * The possible types of EX load slots.
 */
enum EXLoadMode {
	CharacterEXLoadMode,
	GearEXLoadMode
};

struct CSSObject {
	fillerData<0xC> filler;
	u8 state;
	fillerData<0x2B> filler4;
	std::array<u8, 4> cssSelectionState;
	fillerData<0x4> filler3;
	std::array<bool, 4> visualTrigger;
	u8 visualTriggerTime;
};

/**
 * These values are only used in the CheckPlayerHoveringOverPossibleEXLoad() function.
 */
enum class EXLoadHoverTypes : u8{
	OnlyCharacter,///< To only check EX loadable characters the Player is hovering over.
	OnlyGear      ///< To only check EX loadable gears the Player is hovering over.
};

struct Player;

[[nodiscard,deprecated]] EnabledEXLoads FetchEnabledEXLoadIDs(const Player &player);

ASMUsed [[deprecated]] void FetchEnabledEXLoadIDs(const Player *player, EnabledEXLoads &exLoads);
ASMUsed bool IsAnyE10EXLoad(const Player &player);
ASMUsed void ApplyEXLoadFancyVisuals(const Player &player, CSSObject &object);
ASMUsed void Character_UpdateGraphicalEXLoad(GraphicalObject *object);
ASMUsed void Gear_UpdateGraphicalEXLoad(GraphicalObject *object);
ASMUsed const char *GetEXLoadFilenameSuperForm(const Player &player, const char *filename);
ASMUsed void _CheckEXLoadHUDColors();
ASMUsed void _CheckEXLoadBoostColors();

void CheckEXLoadHUDColors(const Player &player, EXLoadColors &returnValues);

[[nodiscard]] EXLoadColors CheckEXLoadHUDColors(const Player &player);

void CheckEXLoadBoostColors(const Player &player, EXLoadColors &returnValues);

bool CheckIfEXLoadCanBeApplied(const Player &player, const EXLoadInfo &info);

ASMUsed void UpdateEXLoadData(Player &player);

void RotateEXLoadAndFetch(Player &player,
						std::span<const EXLoadInfo> exLoadDataSlots,
						const EXLoadMode &exLoadMode,
						const EXLoadTypes &exLoadType);

void ApplyNextEXLoad(Player &player, const EXLoadMode &exLoadMode, const EXLoadTypes &exLoadType);

void CheckPlayerEXLoadButton(Player &player, const EXLoadMode &exLoadMode);

std::optional<std::string> GetEXLoadCharacterModel(const Player &player, GearType gearType);
std::optional<std::string> GetEXLoadSkateModelStart(const Player &player);
bool CheckPlayerHoveringOverPossibleEXLoad(const Player &player, EXLoadHoverTypes type);

ASMUsed void HatsuneMiku_BindAttackPartsToBone(Player &player, Matrix3x3F &savePtr, u8 level);

ASMUsed [[nodiscard]] bool CheckPlayerHasCharacterEXLoadID(const Player &player, EXLoad exLoad);
ASMUsed [[nodiscard]] bool CheckPlayerHasGearEXLoadID(const Player &player, EXLoad exLoad);

ASMUsed void ClearPlayerEXLoadBSS();
ASMUsed void ClearSpecificEXLoads();