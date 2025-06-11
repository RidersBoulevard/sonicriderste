#define ASMHELPER_MAINFUNCNAME CallPlayerAttackRangeTweaks
#define ASMHELPER_FUNCARGUMENTSETUP \
    "fmr f1, f0\n"                  \
    "mr r3, r30\n"
#define ASMHELPER_FUNCTOCALL Player_AttackRangeTweaks

#include "globalDefs.hpp"
#include "lib/ASMHelper.hpp"
#include "riders/player.hpp"
#include "lib/stdlib.hpp"

[[nodiscard]] f32 Player_AttackRangeTweaks(Player &player, f32 currentRange) {
    constexpr f32 DEFAULT_RANGE = 3.0f;
    constexpr f32 COMBATCHAR_RANGE = 4.0f;

    constexpr std::array<f32, 2> neoMetalSonicRanges = {3.0, 4.0};

    if (player.isSuperCharacter(Character::Enum::MetalSonic)) {
        const auto rangeIdx = clamp(static_cast<s32>(player.superFormState), 0, 1);
        return neoMetalSonicRanges[static_cast<u32>(rangeIdx)];
    }

    if (player.isSuperCharacter(Character::Enum::Shadow)) {
        return DEFAULT_RANGE;
    }

    if (player.characterArchetype == CharacterArchetype::Combat && player.level == 2) {
        currentRange = COMBATCHAR_RANGE;
    }

    if (player.extremeGear == ExtremeGear::Berserker) {
        if (currentRange > DEFAULT_RANGE || player.state != PlayerState::RailGrind && player.attackProperties->isAOE) {
            currentRange += 0.5f;
        } else if (player.state != PlayerState::RailGrind) {
            currentRange += 1.0f;
        }
    }

    return currentRange;
}

ASMHELPER_FUNCTION