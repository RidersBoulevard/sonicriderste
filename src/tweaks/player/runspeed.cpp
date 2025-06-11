#include "runspeed.hpp"
#include "macros.h"
#include "cosmetics/player/exloads.hpp"

constexpr std::array<f32, 3> BaseRunSpeeds = {
        pSpeed(150), // level 1
        pSpeed(160), // level 2
        pSpeed(170), // level 3
};

constexpr std::array<f32, 4> TypeAdditiveRunSpeeds = {
        pSpeed(20), // speed
        pSpeed(10), // fly
        pSpeed(0), // power
        pSpeed(25) // no type
};

constexpr std::array<f32, 3> AdvantageSRunSpeeds = {
        pSpeed(205), // level 1
        pSpeed(220), // level 2
        pSpeed(240), // level 3
};

constexpr std::array<f32, 4> AdvantageSTypeAdditiveRunSpeeds = {
        pSpeed(5), // speed
        pSpeed(2), // fly
        pSpeed(0), // power
        pSpeed(0) // no type
};

constexpr std::array<f32, 2> SuperNeoMetalRunSpeeds = {
        pSpeed(200), // base form
        pSpeed(230), // super form
};

constexpr std::array<f32, 2> SuperTailsRunSpeeds = {
        pSpeed(180), // base form
        pSpeed(200), // super form
};

/**
 * Used for setting custom run speeds for the Player.
 *
 * @param player The Player to apply the run speed to.
 * @param runSpeed The vanilla run speed.
 * @return New run speed.
 */
f32 Player_RunSpeed(const Player &player, const f32 runSpeed) {
    if (player.state != PlayerState::Run) {
		return runSpeed;
	}

	if(player.extremeGear == ExtremeGear::ChaosEmerald){
		switch(player.character){
			case Character::MetalSonic:
				return SuperNeoMetalRunSpeeds[player.superFormState];
			case Character::SuperSonic:
				if (player.gearExload().exLoadID == EXLoad::HyperSonic){
					return pSpeed(215.0f);
				} // else
				return pSpeed(205.0f);
			case Character::Knuckles:
				return pSpeed(190.0f);
			case Character::Shadow:
				return pSpeed(215.0f);
			case Character::Tails:
				return SuperTailsRunSpeeds[player.superFormState != 0 ? 1 : 0];
			default:
				break;
		}
	}
	if (player.extremeGear == ExtremeGear::AdvantageS) {
        return AdvantageSRunSpeeds[player.level] + AdvantageSTypeAdditiveRunSpeeds[std::to_underlying(player.characterptr->type)];
    }

	const f32 &newRunSpeed = BaseRunSpeeds[player.level];
	f32 extraSpeed{};
	switch (player.character) {
		case Character::Sonic:
			if (player.level == 2) {
				extraSpeed = pSpeed(30);
			} else {
				extraSpeed = TypeAdditiveRunSpeeds[std::to_underlying(player.characterptr->type)];
			}
			break;
		// case Character::E10G:
		// 	extraSpeed = pSpeed(10);
		// 	break;
		default:
		    if (player.hasCharacterExload() && player.characterExload().characterType != CharacterType::DefaultTypeCharacter) {
		        extraSpeed = TypeAdditiveRunSpeeds[std::to_underlying(player.characterExload().characterType)];
		    } else extraSpeed = TypeAdditiveRunSpeeds[std::to_underlying(player.characterptr->type)];
			break;
	}

    return newRunSpeed + extraSpeed;
}

/// Injects Player_RunSpeed() into ASM.
ASMUsed void Call_Player_RunSpeed_1() {
    asm(
            "stwu r1, -0xA0 (r1)\n"
            "stw r0, 0x8 (r1)\n"
            "mflr r0\n"
            "stw r0, 0xA4 (r1)\n"
            "stmw r3, 0xC (r1)\n"
            "stfs f5, 0x90 (r1)\n"
            "fmr f1, f4\n"
            "mr r3, r28\n"
            "bl %[RunSpeed]\n"
            "lwz r0, 0xA4 (r1)\n"
            "mtlr r0\n"
            "lwz r0, 0x8 (r1)\n"
            "lmw r3, 0xC (r1)\n"
            "lfs f5, 0x90 (r1)\n"
            "fmr f4, f1\n"
            "addi r1, r1, 0xA0\n"
            :
            : [RunSpeed] "i"(Player_RunSpeed)
            );
}

/// Injects Player_RunSpeed() into ASM.
ASMUsed void Call_Player_RunSpeed_2() {
    asm(
            "stwu r1, -0xB0 (r1)\n"
            "stw r0, 0x8 (r1)\n"
            "mflr r0\n"
            "stw r0, 0xB4 (r1)\n"
            "stmw r3, 0xC (r1)\n"
            "stfs f1, 0x90 (r1)\n"
            "stfs f2, 0x94 (r1)\n"
            "stfs f3, 0x98 (r1)\n"
            "stfs f4, 0x9C (r1)\n"
            "stfs f5, 0xA0 (r1)\n"
            "fmr f1, f0\n"
            "mr r3, r28\n"
            "bl %[RunSpeed]\n"
            "fmr f0, f1\n"
            "stfs f0, 0xAB4 (r28)\n"
            "lwz r0, 0xB4 (r1)\n"
            "mtlr r0\n"
            "lwz r0, 0x8 (r1)\n"
            "lmw r3, 0xC (r1)\n"
            "lfs f1, 0x90 (r1)\n"
            "lfs f2, 0x94 (r1)\n"
            "lfs f3, 0x98 (r1)\n"
            "lfs f4, 0x9C (r1)\n"
            "lfs f5, 0xA0 (r1)\n"
            "addi r1, r1, 0xB0\n"
            :
            : [RunSpeed] "i"(Player_RunSpeed)
    );
}