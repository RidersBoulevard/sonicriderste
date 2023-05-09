#pragma once

#include "context.hpp"

extern s32 gas32GettingAttackedVoice[];
extern s32 gas32TrickJumpVoice[];
extern s32 CharacterPassingPlayerVoice[];

constexpr std::array<s32, EXLoadCount> EXLoadGettingAttackedVoice = {
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1, // miku
        -1,
        -1, // reala
        -1, // gongon
};

constexpr std::array<s32, EXLoadCount> EXLoadTrickJumpVoice = {
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        static_cast<int>(0xA9200700), // miku
        -1,
        -1, // reala
        static_cast<int>(0xA91F0900), // gongon
};

constexpr std::array<s32, EXLoadCount> EXLoadCharacterPassingVoice = {
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        static_cast<int>(0xA9200A00), // miku
        -1,
        -1, // reala
        static_cast<int>(0xA91F0C00), // gongon
};

ASMUsed void PlayPassingCharacterVoice(Player *player);
global void PlayGettingAttackedVoice(Player *player);
ASMUsed void PlayTrickJumpVoice(Player *player);