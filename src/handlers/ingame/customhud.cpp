#include "customhud.hpp"
#include "customtext.hpp"
#include "lib/lib.hpp"
#include "lib/stdlib.hpp"
#include "main.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "riders/gamemode.hpp"
#include <cmath>

constinit HUDStruct EmerlTypeHUD_1P{
        0x2C,
        0x74,
        0,
        0,
        0x2B,
        0x2B,
        0.0234375f
};

constinit std::array<HUDStruct, 2> EmerlTypeHUD_2P = {{
        {0x24,
         0x54,
         0,
         0,
         0x2B,
         0x2B,
         0.0234375f},

        {0x24,
         0x144,
         0,
         0,
         0x2B,
         0x2B,
         0.0234375f}
}};

constinit std::array<HUDStruct, 4> EmerlTypeHUD_4P = {{
        {0x3A,
         0x80,
         0,
         0,
         0x2B,
         0x2B,
         0.0234375f},

        {0x21C,
         0x80,
         0,
         0,
         0x2B,
         0x2B,
         0.0234375f},

        {0x3A,
         0x162,
         0,
         0,
         0x2B,
         0x2B,
         0.0234375f},

        {0x21C,
         0x162,
         0,
         0,
         0x2B,
         0x2B,
         0.0234375f}
}};

constexpr std::array<u32, 3> EmerlTypeColors = {
        0x0080FF00,// speed
        0xFFFF0000,// fly
        0xFF200000 // power
};

extern const vu32 gu32ViewportNum;

HUDStruct &GetEmerlTypeIconHUDStruct(const Player &player) {
	const u32 playerAmount = clamp(gu32ViewportNum, 0U, 4U);

	switch(playerAmount) {
		case 1:
			return EmerlTypeHUD_1P;
		case 2:
			return EmerlTypeHUD_2P[player.index];
		case 3 ... 4:
		default:
			return EmerlTypeHUD_4P[player.index];
	}
}

ASMUsed void Player_EmerlRenderTypeIcon() {
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != TimeTrial) {
		return;
	}

	ObjectNode *object = gpsCurrentTask;
	auto *object1 = static_cast<EmerlTypeIconObject1 *>(object->object);

	switch(object->state) {
		default:
			break;
		case 2:
			TRK_memset(&object1->opacity, 0, sizeof(object1->opacity));
			TRK_memset(&object1->isFullyOpaque, 0, sizeof(object1->isFullyOpaque));
			object->state += 1;
			[[fallthrough]];
		case 3:
			auto playerAmount = clamp(InGamePlayerCount, 0U, 4U);

			for(u32 i = 0; i < playerAmount; i++) {
				const Player &player = players[i];
				const auto &playerIndex = player.index;

				if(player.character != CharacterEnums::Emerl) { continue; }

				s32 newOpacity;
				if(Player_IsHUDVisible(player)) {
					newOpacity = object1->opacity[playerIndex] + 8;
				} else {
					if(gu32Game_PauseFlag) {
						newOpacity = object1->opacity[playerIndex] - 32;
					} else {
						newOpacity = object1->opacity[playerIndex] - 8;
					}
				}

				newOpacity = clamp(newOpacity, 0x0, 0xFF);
				object1->opacity[playerIndex] = static_cast<u8>(newOpacity);

				u32 textureRGBA;
				const auto &type = player.typeAttributes;
				if(type.hasAny(Type::Power)){
					textureRGBA = EmerlTypeColors[2];
				} else if(type.hasAny(Type::Fly)){
					textureRGBA = EmerlTypeColors[1];
				} else if(type.hasAny(Type::Speed)){
					textureRGBA = EmerlTypeColors[0];
				} else {
					textureRGBA = 0xFFFFFF00;
				}

				const GizoidReplicationInfo &grInfo = PlayerGizoidReplication[playerIndex];

				if(grInfo.timer > 0 && grInfo.timer < toFrames(2)) {
					if((gu32GameCnt % 15) == 0u) {
						object1->isFullyOpaque[playerIndex] ^= 1;
					}

					if(!object1->isFullyOpaque[playerIndex]) {
						newOpacity = 0;
					}
				}

				HUDStruct &hud = GetEmerlTypeIconHUDStruct(player);
				textureRGBA |= static_cast<u8>(newOpacity);
				Custom_CreateHUDElement(texList_GTSE_0, 8, textureRGBA, &hud);
			}
			break;
	}
}