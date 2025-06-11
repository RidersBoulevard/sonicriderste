#include <cmath>
#include "containers/vector3.hpp"
#include "cosmetics/player/exloads.hpp"
#include "riders/player.hpp"
#include "lib/stdlib.hpp"

// acronym "Trs" in this case stands for translation, rotation, scale
struct NodeTrs {
    Vector3F translation;
    Quaternion rotation;
    Vector3F scale;
};
static_assert(sizeof(NodeTrs) == 0x28);

ASMDefined s8 lbl_10087CF4;

ASMDefined void nnMultiplyQuaternion(void *, void *, void *);
ASMDefined void nnMakeRotateZXYQuaternion(void *, s32, s32, s32);

// for rigs that aren't eggman
constexpr auto QUILL_ROOT = 11;
constexpr auto QUILL_MIDDLE = 12;
constexpr auto QUILL_END = 13;

constexpr auto HAIR_L_ROOT = 14;
constexpr auto HAIR_L_MIDDLE = 15;
constexpr auto HAIR_L_END = 16;
constexpr auto HAIR_R_ROOT = 17;
constexpr auto HAIR_R_MIDDLE = 18;
constexpr auto HAIR_R_END = 19;

ASMUsed void Silver_QuillMovement(Player *player, std::array<NodeTrs, 71> &nodeTrsList) {
	f32 boneWeight;
	f64 unkn;
	u32 gameCnt = gu32GameCnt;
	s32 unkn2, unkn3;
	Quaternion test{};

	if(player->speed > pSpeed(250)) {
		boneWeight = 1102.55f;
	} else {
		boneWeight = player->speed * 927.375f;
	}

	if(!gu32Game_PauseFlag) lbl_10087CF4 = static_cast<s8>(player->unknownVal);
	unkn = std::sin(gameCnt * 0.54f);
	unkn2 = lbl_10087CF4 + 15;
	unkn3 = unkn2 * (6.0f * player->speed);
	unkn2 = boneWeight * (f32) unkn;

	nnMakeRotateZXYQuaternion(&test, 0, -unkn3, unkn2);
	nnMultiplyQuaternion(&nodeTrsList[QUILL_ROOT].rotation, &nodeTrsList[QUILL_ROOT].rotation, &test);

	unkn = std::sin((gameCnt - 15) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&nodeTrsList[QUILL_MIDDLE].rotation, &nodeTrsList[QUILL_MIDDLE].rotation, &test);

	unkn = std::sin((gameCnt - 30) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&nodeTrsList[QUILL_END].rotation, &nodeTrsList[QUILL_END].rotation, &test);
}

ASMUsed void Blaze_QuillMovement(Player *player, std::array<NodeTrs, 71> &nodeTrsList) {
	f32 boneWeight;
	f64 unkn;
	u32 gameCnt = gu32GameCnt;
	s32 unkn2, unkn3;
	Quaternion test{};

	if(player->speed > pSpeed(250)) {
		boneWeight = 2941.5f;
	} else {
		boneWeight = player->speed * 2685.5f;
	}

	if(!gu32Game_PauseFlag) lbl_10087CF4 = static_cast<s8>(player->unknownVal);
	unkn = std::sin(gameCnt * 0.54f);
	unkn2 = lbl_10087CF4 + 15;
	unkn3 = unkn2 * (6.0f * player->speed);
	unkn2 = boneWeight * (f32) unkn;

	nnMakeRotateZXYQuaternion(&test, 0, -unkn3, unkn2);
	nnMultiplyQuaternion(&nodeTrsList[QUILL_ROOT].rotation, &nodeTrsList[QUILL_ROOT].rotation, &test);

	unkn = std::sin((gameCnt - 15) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&nodeTrsList[QUILL_MIDDLE].rotation, &nodeTrsList[QUILL_MIDDLE].rotation, &test);

	unkn = std::sin((gameCnt - 30) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&nodeTrsList[QUILL_END].rotation, &nodeTrsList[QUILL_END].rotation, &test);
}

ASMUsed void MikuCharacterFeatures(Player *player, std::array<NodeTrs, 71> &nodeTrsList) {
	if(player->characterExload().exLoadID == EXLoad::HatsuneMiku) { Blaze_QuillMovement(player, nodeTrsList); }
}

template <typename T>
constexpr T degreesToBams(f32 deg) {
    return static_cast<T>(deg * (65536.0F / 360.0F));
}

ASMUsed void ScarfPhysics(const Player &player, std::array<NodeTrs, 71> &nodeTrsList) {
    s32 rootRotation;
    Quaternion quat{};

    if (player.speed == 0.0F) {
        rootRotation = degreesToBams<s32>(40);
    } else if (player.speed >= 0.5F) {
        rootRotation = degreesToBams<s32>(15);
    } else {
        f32 const var = -(2.0F * player.speed - 1.0F);
        rootRotation = static_cast<s32>(var * degreesToBams<s32>(5) + degreesToBams<s32>(5));
    }

    auto speedMultiplier = clamp(player.speed * 2500.0F, 0.0F, 4000.0F);
    s32 newRotation = rootRotation + static_cast<s32>(speedMultiplier * std::sin(gu32GameCnt * 0.5F));

    nnMakeRotateZXYQuaternion(&quat, 0, 0, -newRotation);
    nnMultiplyQuaternion(&nodeTrsList[QUILL_ROOT].rotation, &nodeTrsList[QUILL_ROOT].rotation, &quat);

    speedMultiplier = clamp(player.speed * 5000.0F, 0.0F, 6500.0F);
    newRotation = static_cast<s32>(speedMultiplier * std::sin((gu32GameCnt - 15) * 0.5F));

    nnMakeRotateZXYQuaternion(&quat, 0, 0, -newRotation);
    nnMultiplyQuaternion(&nodeTrsList[QUILL_MIDDLE].rotation, &nodeTrsList[QUILL_MIDDLE].rotation, &quat);

    newRotation = static_cast<s32>(speedMultiplier * std::sin((gu32GameCnt - 30) * 0.5F));

    nnMakeRotateZXYQuaternion(&quat, 0, 0, -newRotation);
    nnMultiplyQuaternion(&nodeTrsList[QUILL_END].rotation, &nodeTrsList[QUILL_END].rotation, &quat);
}

ASMUsed void MikuHairPhysics(const Player &player, std::array<NodeTrs, 71> &nodeTrsList) {
    // Behavior properties
    constexpr f32 SIN_AMPLITUDE = 0.4f; // Controls the sine wave's amplitude.
    constexpr f32 FREQUENCY_FAST = 2300.04f;
    constexpr f32 FREQUENCY_NORMAL = 2000.0f;
    constexpr f32 MAX_ROOT_ROTATION_FAST = 54.4444f;
    constexpr f32 MAX_ROOT_ROTATION_NORMAL = 50.0f;

    // Speed that is considered "fast" and where frequency and root rotation gets capped.
    // If the player is past this speed, FREQUENCY_FAST and MAX_ROOT_ROTATION_FAST get used,
    // instead of taking player speed into consideration.
    constexpr f32 FAST_SPEED = pSpeed(250);

    Quaternion quat{};
    f32 speedMultiplier;
    f32 rootMultiplier;

    if (player.speed > FAST_SPEED) {
        speedMultiplier = FREQUENCY_FAST;
        rootMultiplier = MAX_ROOT_ROTATION_FAST;
    } else {
        speedMultiplier = player.speed * FREQUENCY_NORMAL;
        rootMultiplier = player.speed * MAX_ROOT_ROTATION_NORMAL;
    }

    // Set root bones' rotation based on speed
    auto rootRotation = degreesToBams<s32>(rootMultiplier);
    auto extraDeg = static_cast<s32>(speedMultiplier * std::sin(gu32GameCnt * SIN_AMPLITUDE));
    nnMakeRotateZXYQuaternion(&quat, 0, rootRotation + extraDeg, 0);
    nnMultiplyQuaternion(&nodeTrsList[HAIR_L_ROOT].rotation, &nodeTrsList[HAIR_L_ROOT].rotation, &quat);

    extraDeg = static_cast<s32>(speedMultiplier * std::sin(gu32GameCnt * SIN_AMPLITUDE));
    nnMakeRotateZXYQuaternion(&quat, 0, extraDeg - rootRotation, 0);
    nnMultiplyQuaternion(&nodeTrsList[HAIR_R_ROOT].rotation, &nodeTrsList[HAIR_R_ROOT].rotation, &quat);

    // Set middle hair bones' rotation based on speed
    extraDeg = static_cast<s32>(speedMultiplier * std::sin((gu32GameCnt - 15) * SIN_AMPLITUDE));
    nnMakeRotateZXYQuaternion(&quat, 0, extraDeg, 0);
    nnMultiplyQuaternion(&nodeTrsList[HAIR_L_MIDDLE].rotation, &nodeTrsList[HAIR_L_MIDDLE].rotation, &quat);
    nnMultiplyQuaternion(&nodeTrsList[HAIR_R_MIDDLE].rotation, &nodeTrsList[HAIR_R_MIDDLE].rotation, &quat);

    // Set end hair bones' rotation based on speed
    extraDeg = static_cast<s32>(speedMultiplier * std::sin((gu32GameCnt - 30) * SIN_AMPLITUDE));
    nnMakeRotateZXYQuaternion(&quat, 0, extraDeg, 0);
    nnMultiplyQuaternion(&nodeTrsList[HAIR_L_END].rotation, &nodeTrsList[HAIR_L_MIDDLE].rotation, &quat);
    nnMultiplyQuaternion(&nodeTrsList[HAIR_R_END].rotation, &nodeTrsList[HAIR_R_MIDDLE].rotation, &quat);
}