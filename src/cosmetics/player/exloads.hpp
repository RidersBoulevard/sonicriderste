/** \file exloads.hpp
 * Header file for dealing with EX loads.
 */

#pragma once

#include "containers/graphicalobject.hpp"
#include "containers/rgba.hpp"
#include "riders/player.hpp"

constexpr auto CharacterEXLoadSlots = 10;
constexpr auto GearEXLoadSlots = 5;

constexpr auto NotLockedGearSelectionID = 0x1A;
constexpr auto LockedGearSelectionID = 0x1B;

/**
 * Stores all of the EX load IDs for every unique EX load in the game.
 */
enum EXLoads : u8 {
    NoneEXLoad,///< This ID is used if no EX load is selected.
    PerfectNazo,
    E10REXLoad,
    DarkSonicEXLoad,
    HyperSonicEXLoad,
    StardustSpeederEXLoad,
    HyperHangOnEXLoad,
    TheProfessionalEXLoad,
    OllieKingGearEXLoad,
    WindmasterJetEXLoad,
    NeoMetalEXLoad,
    E99EXLoad,
    TheBeastEXLoad,
    E10BEXLoad,
    HatsuneMikuEXLoad,
    MetalSonicNoScarfEXLoad,
    RealaEXLoad,
    GonGonEXLoad,
    WCJet,
    WCStorm,
    WCWave,
    MCSonic,
    MCTails,
    MCKnuckles,
    SuperStormEXLoad,
    PyroEXLoad,
    HangOnATEXLoad,
    SelkadoomEXLoad,
    SSEXLoad,
    // add ex loads before the EXLoadCount enum please kthx
    EXLoadCount
};

ASMDefined void lbl_0013FA84(u32 soundID); /* asm, some different version of PlaySoundFromDAT function */

struct EXLoadBoostColor {
    RGBANormalized rgbaFirstLayer; // first layer is the primary one, you see this one 90% of the time
    RGBANormalized rgbaSecondLayer; // this second layer starts flashing right as your boost is about to end
};

/**
 * Contains all required information and other miscellaneous things for the current EX load the Player has selected, if applicable.
 * Refer to #Player_EXLoadData for the variable that holds this information at runtime.
 */
struct CurrentEXLoadInfo {
    u8 exLoadIndex = 0;    ///< index to the current EXLoadInfo in use
    u8 exLoadDelayTime = 0;///< how many frames of delay before next input is read again, counts down to 0
};

struct EnabledEXLoads {
    EXLoads characterExLoadID = NoneEXLoad;
    EXLoads gearExLoadID = NoneEXLoad;

    [[nodiscard]] constexpr bool isExloadEnabled() const {
        return (characterExLoadID | gearExLoadID) != 0;
    }
};

struct EXLoadColors {
    void *colors = nullptr;
    u32 index = 0;
};

enum CharacterType : s8 {
    DefaultTypeCharacter = -1,
    SpeedCharacter,
    FlyCharacter,
    PowerCharacter,
    CPUCharacter,
};

struct EXLoadInfo {
    s8 exLoadType;        ///< Whether is a L load or a X load, or no ex load
    EXLoads exLoadID;     ///< taken from enum ::EXLoads
    s8 character;         ///< character it applies to, if none, it'll be -1 (0xFF)
    s8 gear;              ///< gear it applies to, if none, it'll be -1 (0xFF)
    u16 portraitTextureID;///< what texture ID it will use for the portrait of the ex load
    u16 gearTextTextureID; ///< what texture ID it will use for gear text if it's a gear ex load. if it's a character ex load, this is used for their default gear specifically. otherwise -1
    u16 characterTextTextureID; ///< what texture ID it will use for character text if it's a character ex load, otherwise -1
    u16 defaultGearPortraitTextureID; ///< what texture ID it will use for character ex loads' default gears if applicable, otherwise -1
    CharacterType characterType; ///< if character ex load, what type they're supposed to be, -1 for default
};

/**
 * These values are only used in the CheckPlayerHoveringOverPossibleEXLoad() function.
 */
enum EXLoadHoverTypes {
    OnlyCharacter,///< To only check EX loadable characters the Player is hovering over.
    OnlyGear      ///< To only check EX loadable gears the Player is hovering over.
};

// LLoadType = for ex loads that are accessed via the L button (cosmetic ex loads)
// EXLoadType = for ex loads that are accessed via the X button (gameplay altering ex loads)
// AllEXLoadTypes... = used for scrolling through both L and X loads
enum EXLoadTypes {
    NoneEXLoadType,
    LLoadType,
    EXLoadType,
    LLoadTypeForwards,
    LLoadTypeBackwards,
    EXLoadTypeForwards,
    EXLoadTypeBackwards,
    AllEXLoadTypes
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

extern m2darray<CurrentEXLoadInfo, MaxPlayerCount, 2> Player_EXLoadData;

constexpr std::array<EXLoadInfo, CharacterEXLoadSlots> CharacterEXLoadDataSlots = {
        {
                {NoneEXLoadType,
                 NoneEXLoad,
                 -1,
                 -1,
                 static_cast<u16>(-1),
                 static_cast<u16>(-1),
                 static_cast<u16>(-1),
                 static_cast<u16>(-1),
                 DefaultTypeCharacter
                },
                {
                        EXLoadType,
                        E10REXLoad,
                        E10G,
                        -1,
                        0xEE,
                        static_cast<u16>(-1),
                        0x103,
                        static_cast<u16>(-1),
                        PowerCharacter
                },
                {
                        LLoadType,
                        NeoMetalEXLoad,
                        MetalSonic,
                        -1,
                        0xEF,
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        EXLoadType,
                        E10BEXLoad,
                        E10G,
                        -1,
                        0xBE, 0x23,
                        0x56, 0xA9,
                        SpeedCharacter
                },
                {
                        EXLoadType,
                        HatsuneMikuEXLoad,
                        Ulala,
                        -1,
                        0xF4, 0xFD,
                        0xFE, 0x101,
                        DefaultTypeCharacter
                },
                {
                        LLoadType,
                        MetalSonicNoScarfEXLoad,
                        MetalSonic,
                        -1,
                        0xF7,
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        EXLoadType,
                        RealaEXLoad,
                        Nights,
                        -1,
                        0xF3, 0xFC,
                        0x100, 0x102,
                        DefaultTypeCharacter
                },
                {
                        EXLoadType,
                        GonGonEXLoad,
                        Aiai,
                        -1,
                        0xF2,
                        static_cast<u16>(-1),
                        0xFF,
                        static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        LLoadType,
                        SelkadoomEXLoad,
                        Shadow,
                        -1,
                        0x107,
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        EXLoadType,
                        SSEXLoad,
                        Sonic,
                        -1,
                        0xA0,
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
        }
};

constexpr std::array<EXLoadInfo, GearEXLoadSlots> GearEXLoadDataSlots = {
        {
                {
                        NoneEXLoadType,
                        NoneEXLoad,
                        -1,
                        -1,
                        static_cast<u16>(-1), static_cast<u16>(-1),
                        static_cast<u16>(-1), static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        LLoadType,
                        PerfectNazo,
                        Sonic,
                        ExtremeGear::ChaosEmerald,
                        0xED, 0x1A,
                        static_cast<u16>(-1), static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        LLoadType,
                        DarkSonicEXLoad,
                        Sonic,
                        ExtremeGear::ChaosEmerald,
                        0xF0, 0x1A,
                        static_cast<u16>(-1), static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        EXLoadType,
                        HyperSonicEXLoad,
                        Sonic,
                        ExtremeGear::ChaosEmerald,
                        0xF1, 0x1A,
                        static_cast<u16>(-1), static_cast<u16>(-1),
                        DefaultTypeCharacter
                },
                {
                        EXLoadType,
                        StardustSpeederEXLoad,
                        -1,
                        ExtremeGear::HighBooster,
                        0xF5, 0xF6,
                        static_cast<u16>(-1), static_cast<u16>(-1),
                        DefaultTypeCharacter
                }
        }
};

constexpr std::array<f32, EXLoadCount> EXLoadWeights = {
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
        -1.0F,
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
        -1.0f // seelkadoom
};

constexpr std::array<CharacterArchetypes, EXLoadCount> EXLoadArchetypes = {
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        CombatArchetype,// hatsune miku
        NoEXLoadArchetype,
        BoostArchetype,// reala
        AllRounder,    // gongon
        Windcatcher,
        Windcatcher,
        Windcatcher,
        Mechanic,
        Mechanic,
        Mechanic,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype,
        NoEXLoadArchetype, // seelkadoom
        SuperSonicArchetype // super sonic exload
};

ASMUsed const std::array<RGBA, EXLoadCount> EXLoadHUDColors;
ASMUsed const std::array<EXLoadBoostColor, EXLoadCount> EXLoadBoostColors;

/**
 * The possible types of EX load slots.
 */
enum EXLoadModes {
    CharacterEXLoadMode,
    GearEXLoadMode
};

[[nodiscard]] EnabledEXLoads FetchEnabledEXLoadIDs(const Player &player);

ASMUsed void FetchEnabledEXLoadIDs(const Player *player, EnabledEXLoads &exLoads);
ASMUsed bool IsAnyE10EXLoad(const Player *player);
ASMUsed void ApplyEXLoadFancyVisuals(const Player &player, CSSObject &object);
ASMUsed void Character_UpdateGraphicalEXLoad(GraphicalObject *object);
ASMUsed void Gear_UpdateGraphicalEXLoad(GraphicalObject *object);
ASMUsed const char *GetEXLoadFilenameSuperForm(Player *player, const char filename[5]);
ASMUsed void _CheckEXLoadHUDColors();
ASMUsed void _CheckEXLoadBoostColors();

void CheckEXLoadHUDColors(const Player *player, EXLoadColors &returnValues);

[[nodiscard]] EXLoadColors CheckEXLoadHUDColors(const Player &player);

void CheckEXLoadBoostColors(const Player *player, EXLoadColors &returnValues);

bool CheckIfEXLoadCanBeApplied(const Player &player, const EXLoadInfo &info);

ASMUsed void UpdateEXLoadData(Player *player);

u32
RotateEXLoadAndFetch(const Player &player, std::span<const EXLoadInfo> exLoadDataSlots, const EXLoadModes &exLoadMode,
                     const EXLoadTypes &exLoadType);

void ApplyNextEXLoad(const Player &player, const EXLoadModes &exLoadMode, const EXLoadTypes &exLoadType);

void CheckPlayerEXLoadButton(const Player &player, const EXLoadModes &exLoadMode);

const char *GetEXLoadCharacterModel(Player *player, GearType gearType);

const char *GetEXLoadSkateModelStart(Player *player);

bool CheckPlayerHoveringOverPossibleEXLoad(const Player &player, EXLoadHoverTypes type);

ASMUsed void HatsuneMiku_BindLeekToBoneLvl3Attack(Player &player, Matrix3x3F &savePtr);
ASMUsed void ClearPlayerEXLoadBSS();
ASMUsed void ClearSpecificEXLoads();