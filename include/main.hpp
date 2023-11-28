#pragma once

#include "globalDefs.hpp"

ASMDefined const vu32 gu32GameCnt; // current game time in frames
ASMDefined const vu32 gu32Game_PauseFlag;

ASMDefined vs32 RuleSettings;

ASMDefined char* DumpPackManFile(const char [], u32);
ASMDefined void SetArchiveBinary(const char [], u32, u32);
ASMDefined void SetupGVRTextureArchive(void *packmanStart, u32 *offsetFromStart, void *saveptr, u32 mallocHi);
ASMDefined void PackMan_SetupRenderedText(void *packmanStart, u32 *offsetFromStart, void *saveptr);