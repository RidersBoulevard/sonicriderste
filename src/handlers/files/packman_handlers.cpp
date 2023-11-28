#include "packman_handlers.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "handlers/music/custom_music.hpp"
#include "lib/stdlib.hpp"
#include "lib/lib.hpp"
#include "main.hpp"
#include <vector>
#include "endian.hpp"
#include "separatemodelloading.hpp"
#include "cosmetics/player/exloads.hpp"

extern std::array<void *, 2> CustomTextArchiveFile;
void *texList_CustomParticles;

// packman IDs 48401 and above are all free to use

struct PackManFileHeader {
	s32 folderCount;
	u8 fileCounts[1]; // supposed to be fileCounts[folderCount]
};

ASMDefined void *ExtraCharacterAnimationFiles;
ASMDefined void *CharacterAssetFilePackageCounts;
ASMDefined void *coau16AttackObjectRadeMode_All;
ASMDefined void *lbl_100091C8;
ASMDefined void *lbl_10008508;
ASMDefined void *lbl_10008548;
ASMDefined void *lbl_001F1294;
ASMDefined void *lbl_100084C8;
ASMDefined void lbl_8007B9F8();
ASMDefined void lbl_8007BD24();
ASMDefined void utilBinSetUpObject();

// all the declarations above are dummy declarations (they're not correct) for the ASM function "PackManHandler_CharacterAssets"

ASMDefined void *lbl_801AFC64;
ASMDefined std::array<void *, MaxPlayerCount> bss_BoardOnlyModelData;
ASMDefined std::array<void *, MaxPlayerCount> bss_BoardOnlyTextures;
ASMDefined void lbl_00047654(void *fileStart, u32 *offsetFromStart, void *modelptr, void *textureptr);

ASMUsed void
PackManHandler_CustomText(void *fileStart, const u16 *fileIndexStart, u32 *fileOffsetStart, u32 currentFolderIndex) {
	// ID : 0x2
	u16 index = *(u16 *) ((u8 *) fileIndexStart + currentFolderIndex);

	PackMan_SetupRenderedText(fileStart, fileOffsetStart + index, &CustomTextArchiveFile[0]);
	SetupGVRTextureArchive(fileStart, fileOffsetStart + (index + 1), &lbl_801AFC64, 0);
    SetupGVRTextureArchive(fileStart, fileOffsetStart + (index + 2), &texList_CustomParticles, 0);
}

ASMUsed void PackManHandler_BoardModel(void *fileStart,
                                       const u16 *fileIndexStart,
                                       u32 *fileOffsetStart,
                                       u32 currentFolderIndex,
                                       u32 playerIndex) {
	// ID : 0x1
	u16 index = *(u16 *) ((u8 *) fileIndexStart + currentFolderIndex);

	lbl_00047654(fileStart,
	             fileOffsetStart + index,
	             &bss_BoardOnlyModelData[playerIndex],
	             &bss_BoardOnlyTextures[playerIndex]);
}

#include "macros.h"
ASMUsed void PackManHandler_CharacterAssets(void *fileStart, u32 *fileOffsetStart, u32 playerIndex) {
	// ID : 0x3, handles every other file that isn't animation files in character animation archive files
	// so this would be extra assets like twist models, misc textures, etc
	asm volatile(// Todo: change to use extended inline syntax
	        "   stwu r1, -0x60(r1)\n"
	        "   mflr r0\n"
	        "   lis r6, players@ha\n"
	        "   stw r0, 0x64(r1)\n"
	        "   mr r0, r5\n"
	        "   mulli r7, r0, 0x1080\n"
	        "   stmw r14, 0x18(r1)\n"
	        "   addi r0, r6, players@l\n"
	        "   mr r15, r3\n"
	        "   add r21, r0, r7\n"
	        "   stw r5, 8(r1)\n"
	        "   lis r5, gsAttackObjReadManager@ha\n"
	        "   mr r16, r4\n"
	        "   addi r5, r5, gsAttackObjReadManager@l\n"
	        "   li r20, 0\n"
	        "   li r19, 0\n"
	        "   li r17, 0\n"
	        "   lbz r3, 0x1029(r21)\n"
	        "   slwi r0, r3, 1\n"
	        "   add r14, r5, r0\n"
	        "   lis r3, CharacterAssetFilePackageCounts@ha\n"
	        "   lbz r0, 1(r14)\n"
	        "   addi r3, r3, CharacterAssetFilePackageCounts@l\n"
	        "   lbzx r0, r3, r0\n"
	        "   stw r0, 0x14(r1)\n"
	        "   lwz r0, 8(r1)\n"
	        "   lis r5, coau16AttackObjectRadeMode_All@ha\n"
	        "   lis r3, lbl_100091C8@ha\n"
	        "   li r18, 0\n"
	        "   mulli r4, r0, 0x3c\n"
	        "   addi r24, r5, coau16AttackObjectRadeMode_All@l\n"
	        "   addi r0, r3, lbl_100091C8@l\n"
	        "   add r23, r0, r4\n"

	        "lbl_00045720:\n"
	        "   lhz r22, 0(r24)\n"
	        "   cmplwi r22, 2\n"
	        "   bne lbl_00045744\n"
	        "   mr r3, r15\n"
	        "   mr r4, r16\n"
	        "   addi r5, r23, 4\n"
	        "   li r6, 0\n"
	        "   bl SetupGVRTextureArchive\n"
	        "   b lbl_000458E8\n"

	        "lbl_00045744:\n"
	        "   clrlwi. r21, r22, 0x1f\n"
	        "   mr r25, r16\n"
	        "   li r27, 0\n"
	        "   beq lbl_000457A4\n"
	        "   lwz r0, 0(r16)\n"
	        "   mr r3, r23\n"
	        "   addi r4, r1, 0x10\n"
	        "   addi r25, r16, 4\n"
	        "   add r5, r15, r0\n"
	        "   bl utilBinSetUpObject\n"
	        "   lwz r3, 0x10(r1)\n"
	        "   li r27, 1\n"
	        "   lwz r3, 0(r3)\n"
	        "   bl func_80053E38\n"
	        "   mr r4, r3\n"
	        "   li r3, 0x10\n"
	        "   addi r0, r4, 0xf\n"
	        "   rlwinm r4, r0, 0, 0, 0x1b\n"
	        "   bl aligned_malloc\n"
	        "   lwz r4, 0x10(r1)\n"
	        "   mr r5, r3\n"
	        "   addi r3, r23, 4\n"
	        "   lwz r4, 0(r4)\n"
	        "   bl func_80053D20\n"

	        "lbl_000457A4:\n"
	        "   rlwinm. r0, r22, 0, 0x1e, 0x1e\n"
	        "   beq lbl_000457FC\n"
	        "   lwz r0, 0(r25)\n"
	        "   addi r25, r25, 4\n"
	        "   lwz r4, 0x10(r1)\n"
	        "   addi r27, r27, 1\n"
	        "   lwz r3, 4(r23)\n"
	        "   add r28, r15, r0\n"
	        "   lwz r31, 4(r4)\n"
	        "   addi r29, r28, 4\n"
	        "   lwz r30, 4(r3)\n"
	        "   lwz r26, 0(r4)\n"

	        "lbl_000457D4:\n"
	        "   lwz r0, 0(r29)\n"
	        "   mr r3, r30\n"
	        "   mr r4, r31\n"
	        "   addi r31, r31, 0x14\n"
	        "   add r5, r28, r0\n"
	        "   addi r30, r30, 0x40\n"
	        "   bl InitTextureStruct\n"
	        "   addic. r26, r26, -1\n"
	        "   addi r29, r29, 4\n"
	        "   bne lbl_000457D4\n"

	        "lbl_000457FC:\n"
	        "   li r3, 0\n"
	        "   rlwinm. r0, r22, 0, 0x1d, 0x1d\n"
	        "   stw r3, 8(r23)\n"
	        "   stw r3, 0xc(r23)\n"
	        "   stw r3, 0x10(r23)\n"
	        "   beq lbl_0004582C\n"
	        "   lwz r0, 0(r25)\n"
	        "   addi r3, r23, 8\n"
	        "   addi r25, r25, 4\n"
	        "   add r4, r15, r0\n"
	        "   bl utilBinSetUpMotion\n"
	        "   addi r27, r27, 1\n"

	        "lbl_0004582C:\n"
	        "   rlwinm. r0, r22, 0, 0x1c, 0x1c\n"
	        "   beq lbl_0004587C\n"
	        "   lwz r0, 0(r25)\n"
	        "   addi r3, r23, 0xc\n"
	        "   add r4, r15, r0\n"
	        "   bl utilBinSetUpMotion\n"
	        "   cmpwi r21, 0\n"
	        "   addi r27, r27, 1\n"
	        "   beq lbl_0004587C\n"
	        "   lwz r3, 0(r23)\n"
	        "   lwz r4, 0xc(r23)\n"
	        "   bl lbl_8007BD24\n"
	        "   mr r4, r3\n"
	        "   li r3, 4\n"
	        "   bl aligned_malloc\n"
	        "   stw r3, 0x10(r23)\n"
	        "   lwz r3, 0x10(r23)\n"
	        "   lwz r4, 0(r23)\n"
	        "   lwz r5, 0xc(r23)\n"
	        "   bl lbl_8007B9F8\n"

	        "lbl_0004587C:\n"
	        "   lhz r0, 0(r24)\n"
	        "   mr r3, r27\n"
	        "   clrlwi. r0, r0, 0x1f\n"
	        "   beq lbl_000458E8\n"
	        "   lwz r4, 0(r23)\n"
	        "   lwz r8, 0x34(r4)\n"
	        "   lwz r0, 0x28(r4)\n"
	        "   subf r6, r8, r20\n"
	        "   orc r7, r20, r8\n"
	        "   subf r4, r0, r19\n"
	        "   orc r5, r19, r0\n"
	        "   srwi r6, r6, 1\n"
	        "   srwi r4, r4, 1\n"
	        "   subf r6, r6, r7\n"
	        "   srwi r6, r6, 0x1f\n"
	        "   subf r4, r4, r5\n"
	        "   srwi r4, r4, 0x1f\n"
	        "   neg r7, r6\n"
	        "   addi r6, r6, -1\n"
	        "   neg r5, r4\n"
	        "   addi r4, r4, -1\n"
	        "   and r7, r20, r7\n"
	        "   and r6, r8, r6\n"
	        "   and r5, r19, r5\n"
	        "   and r0, r0, r4\n"
	        "   or r20, r7, r6\n"
	        "   or r19, r5, r0\n"

	        "lbl_000458E8:\n"
	        "   addi r18, r18, 1\n"
	        "   slwi r0, r3, 2\n"
	        "   cmplwi r18, 3\n"
	        "   add r17, r17, r3\n"
	        "   add r16, r16, r0\n"
	        "   addi r24, r24, 2\n"
	        "   addi r23, r23, 0x14\n"
	        "   blt lbl_00045720\n"
	        "   mulli r18, r20, 0x30\n"
	        "   slwi r0, r19, 2\n"
	        "   li r3, 0x10\n"
	        "   add r4, r18, r0\n"
	        "   bl aligned_malloc\n"
	        "   lwz r0, 8(r1)\n"
	        "   lis r4, lbl_10008508@ha\n"
	        "   addi r4, r4, lbl_10008508@l\n"
	        "   li r29, 0\n"
	        "   slwi r31, r0, 3\n"
	        "   lwz r0, 0x14(r1)\n"
	        "   stwx r3, r4, r31\n"
	        "   add r3, r4, r31\n"
	        "   cmplwi r0, 0\n"
	        "   li r28, 0\n"
	        "   lwzx r0, r4, r31\n"
	        "   add r0, r0, r18\n"
	        "   stw r0, 4(r3)\n"
	        "   beq lbl_00045BA0\n"
	        "   lwz r0, 8(r1)\n"
	        "   lis r5, lbl_001F1294@ha\n"
	        "   lbz r6, 1(r14)\n"
	        "   cmpwi r6, 0x11\n"// opa opa animation
	        "   beq- lbl_SkipOpa\n"
	        "   addi r16, r16, 0x8\n"// skip opa opa weight model asset, if not using opa opa

	        "lbl_SkipOpa:\n"
	        "   lis r3, lbl_10008548@ha\n"
	        "   mulli r4, r0, 0x190\n"
	        "   addi r5, r5, lbl_001F1294@l\n"
	        "   slwi r6, r6, 2\n"
	        "   addi r0, r3, lbl_10008548@l\n"
	        "   lwzx r24, r5, r6\n"
	        "   mr r30, r29\n"
	        "   add r25, r0, r4\n"
	        "   b lbl_00045B64\n"

	        "lbl_00045984:\n"
	        "   lhz r26, 0(r24)\n"
	        "   cmplwi r26, 2\n"
	        "   bne lbl_000459A8\n"
	        "   mr r3, r15\n"
	        "   mr r4, r16\n"
	        "   addi r5, r25, 4\n"
	        "   li r6, 0\n"
	        "   bl SetupGVRTextureArchive\n"
	        "   b lbl_00045B4C\n"

	        "lbl_000459A8:\n"
	        "   clrlwi. r27, r26, 0x1f\n"
	        "   mr r23, r16\n"
	        "   li r21, 0\n"
	        "   beq lbl_00045A08\n"
	        "   lwz r0, 0(r16)\n"
	        "   mr r3, r25\n"
	        "   addi r4, r1, 0xc\n"
	        "   addi r23, r16, 4\n"
	        "   add r5, r15, r0\n"
	        "   bl utilBinSetUpObject\n"
	        "   lwz r3, 0xc(r1)\n"
	        "   li r21, 1\n"
	        "   lwz r3, 0(r3)\n"
	        "   bl func_80053E38\n"
	        "   mr r4, r3\n"
	        "   li r3, 0x10\n"
	        "   addi r0, r4, 0xf\n"
	        "   rlwinm r4, r0, 0, 0, 0x1b\n"
	        "   bl aligned_malloc\n"
	        "   lwz r4, 0xc(r1)\n"
	        "   mr r5, r3\n"
	        "   addi r3, r25, 4\n"
	        "   lwz r4, 0(r4)\n"
	        "   bl func_80053D20\n"

	        "lbl_00045A08:\n"
	        "   rlwinm. r0, r26, 0, 0x1e, 0x1e\n"
	        "   beq lbl_00045A60\n"
	        "   lwz r0, 0(r23)\n"
	        "   addi r23, r23, 4\n"
	        "   lwz r4, 0xc(r1)\n"
	        "   addi r21, r21, 1\n"
	        "   lwz r3, 4(r25)\n"
	        "   add r20, r15, r0\n"
	        "   lwz r14, 4(r4)\n"
	        "   addi r19, r20, 4\n"
	        "   lwz r18, 4(r3)\n"
	        "   lwz r22, 0(r4)\n"

	        "lbl_00045A38:\n"
	        "   lwz r0, 0(r19)\n"
	        "   mr r3, r18\n"
	        "   mr r4, r14\n"
	        "   addi r14, r14, 0x14\n"
	        "   add r5, r20, r0\n"
	        "   addi r18, r18, 0x40\n"
	        "   bl InitTextureStruct\n"
	        "   addic. r22, r22, -1\n"
	        "   addi r19, r19, 4\n"
	        "   bne lbl_00045A38\n"

	        "lbl_00045A60:\n"
	        "   li r3, 0\n"
	        "   rlwinm. r0, r26, 0, 0x1d, 0x1d\n"
	        "   stw r3, 8(r25)\n"
	        "   stw r3, 0xc(r25)\n"
	        "   stw r3, 0x10(r25)\n"
	        "   beq lbl_00045A90\n"
	        "   lwz r0, 0(r23)\n"
	        "   addi r3, r25, 8\n"
	        "   addi r23, r23, 4\n"
	        "   add r4, r15, r0\n"
	        "   bl utilBinSetUpMotion\n"
	        "   addi r21, r21, 1\n"

	        "lbl_00045A90:\n"
	        "   rlwinm. r0, r26, 0, 0x1c, 0x1c\n"
	        "   beq lbl_00045AE0\n"
	        "   lwz r0, 0(r23)\n"
	        "   addi r3, r25, 0xc\n"
	        "   add r4, r15, r0\n"
	        "   bl utilBinSetUpMotion\n"
	        "   cmpwi r27, 0\n"
	        "   addi r21, r21, 1\n"
	        "   beq lbl_00045AE0\n"
	        "   lwz r3, 0(r25)\n"
	        "   lwz r4, 0xc(r25)\n"
	        "   bl lbl_8007BD24\n"
	        "   mr r4, r3\n"
	        "   li r3, 4\n"
	        "   bl aligned_malloc\n"
	        "   stw r3, 0x10(r25)\n"
	        "   lwz r3, 0x10(r25)\n"
	        "   lwz r4, 0(r25)\n"
	        "   lwz r5, 0xc(r25)\n"
	        "   bl lbl_8007B9F8\n"

	        "lbl_00045AE0:\n"
	        "   lhz r0, 0(r24)\n"
	        "   mr r3, r21\n"
	        "   clrlwi. r0, r0, 0x1f\n"
	        "   beq lbl_00045B4C\n"
	        "   lwz r4, 0(r25)\n"
	        "   lwz r8, 0x34(r4)\n"
	        "   lwz r0, 0x28(r4)\n"
	        "   subf r6, r8, r28\n"
	        "   orc r7, r28, r8\n"
	        "   subf r4, r0, r29\n"
	        "   orc r5, r29, r0\n"
	        "   srwi r6, r6, 1\n"
	        "   srwi r4, r4, 1\n"
	        "   subf r6, r6, r7\n"
	        "   srwi r6, r6, 0x1f\n"
	        "   subf r4, r4, r5\n"
	        "   srwi r4, r4, 0x1f\n"
	        "   neg r7, r6\n"
	        "   addi r6, r6, -1\n"
	        "   neg r5, r4\n"
	        "   addi r4, r4, -1\n"
	        "   and r7, r28, r7\n"
	        "   and r6, r8, r6\n"
	        "   and r5, r29, r5\n"
	        "   and r0, r0, r4\n"
	        "   or r28, r7, r6\n"
	        "   or r29, r5, r0\n"

	        "lbl_00045B4C:\n"
	        "   slwi r0, r3, 2\n"
	        "   add r17, r17, r3\n"
	        "   add r16, r16, r0\n"
	        "   addi r24, r24, 2\n"
	        "   addi r25, r25, 0x14\n"
	        "   addi r30, r30, 1\n"

	        "lbl_00045B64:\n"
	        "   lwz r0, 0x14(r1)\n"
	        "   cmplw r30, r0\n"
	        "   blt lbl_00045984\n"
	        "   mulli r14, r28, 0x30\n"
	        "   slwi r0, r29, 2\n"
	        "   li r3, 0x10\n"
	        "   add r4, r14, r0\n"
	        "   bl aligned_malloc\n"
	        "   lis r4, lbl_100084C8@ha\n"
	        "   addi r4, r4, lbl_100084C8@l\n"
	        "   stwx r3, r4, r31\n"
	        "   add r3, r4, r31\n"
	        "   lwzx r0, r4, r31\n"
	        "   add r0, r0, r14\n"
	        "   stw r0, 4(r3)\n"

	        "lbl_00045BA0:\n"
	        "   mr r3, r17\n"
	        "   lmw r14, 0x18(r1)\n"
	        "   lwz r0, 0x64(r1)\n"
	        "   mtlr r0\n"
	        "   addi r1, r1, 0x60\n");
}

ASMUsed void PackManHandler_CharacterAnimation(void *fileStart, u32 *fileOffsetStart, u32 playerIndex) {
	asm volatile(// Todo: change to use extended inline syntax
	        "   stwu r1, -0x60(r1)\n"
	        "   mflr r0\n"
	        "   lis r6, players@ha\n"
	        "   stw r0, 0x64(r1)\n"
	        "   mr r0, r5\n"
	        "   mulli r7, r0, 0x1080\n"
	        "   stmw r14, 0x18(r1)\n"
	        "   addi r0, r6, players@l\n"
	        "   mr r15, r3\n"
	        "   add r21, r0, r7\n"
	        "   stw r5, 8(r1)\n"
	        "   lis r5, gsAttackObjReadManager@ha\n"
	        "   mr r16, r4\n"
	        "   addi r5, r5, gsAttackObjReadManager@l\n"
	        "   li r20, 0\n"
	        "   li r19, 0\n"
	        "   li r17, 0\n"
	        "   lbz r0, 0xbb(r21)\n"
	        "   lbz r3, 0x1029(r21)\n"
	        "   extsb r0, r0\n"
	        "   cmpwi r0, 0x16\n"
	        "   slwi r0, r3, 1\n"
	        "   add r14, r5, r0\n"
	        "   bne lbl_00045640\n"
	        "   lbz r3, 0xba(r21)\n"
	        // default new characters to sonic for now, character id 0x11 is silver
	        // this sets up character animations for opa opa specifically, P?P file
	        "   cmpwi r3, 0x10\n"
	        "   ble- lbl_00045628\n"
	        "   subi r3, r3, 0x11\n"
	        "   lis r4, ExtraCharacterAnimationFiles@ha\n"
	        "   addi r4, r4, ExtraCharacterAnimationFiles@l\n"
	        "   lbzx r3, r3, r4\n"

	        "lbl_00045628:\n"
	        "   li r0, 0x11\n"
	        "   mulli r3, r3, 5\n"
	        "   addi r3, r3, 3\n"
	        "   stb r3, 0(r14)\n"
	        "   stb r0, 1(r14)\n"
	        "   b lbl_00045658\n"

	        "lbl_00045640:\n"
	        "   lbz r3, 0xba(r21)\n"
	        // default new characters to sonic for now, character id 0x11 is silver
	        // this sets up character animations and other stuff, namely their P?O files
	        "   cmpwi r3, 0x10\n"
	        "   ble- lbl_00045644\n"
	        "   cmpwi r3, 0x12\n"// metal sonic
	        "   bne- lbl_NewCharacterAnimationFiles\n"
	        "   lbz r4, 0xBB (r21)\n"
	        "   cmpwi r4, 0x12\n"// chaos emerald
	        "   bne- lbl_NewCharacterAnimationFiles\n"
	        "   li r3, 0xB\n"// super sonic file
	        "   b lbl_00045644\n"

	        "lbl_NewCharacterAnimationFiles:\n"
	        "   subi r3, r3, 0x11\n"
	        "   lis r4, ExtraCharacterAnimationFiles@ha\n"
	        "   addi r4, r4, ExtraCharacterAnimationFiles@l\n"
	        "   lbzx r3, r3, r4\n"

	        "lbl_00045644:\n"
	        "   lbz r4, 0xbe(r21)\n"
	        "   mulli r0, r3, 5\n"
	        "   add r0, r4, r0\n"
	        "   stb r0, 0(r14)\n"
	        "   stb r3, 1(r14)\n"

	        "lbl_00045658:\n"
	        "   lis r3, CharacterAssetFilePackageCounts@ha\n"
	        "   lbz r0, 1(r14)\n"
	        "   lbz r6, 0xbe(r21)\n"
	        "   addi r3, r3, CharacterAssetFilePackageCounts@l\n"
	        "   lbzx r0, r3, r0\n"
	        "   cmplwi r6, 3\n"
	        "   stw r0, 0x14(r1)\n"
	        "   beq lbl_00045700\n"
	        "   lbz r0, 0(r14)\n"
	        "   lis r3, lbl_001F1140@ha\n"
	        "   lis r4, tu16ExclusiveMotionNo@ha\n"
	        "   lis r5, gpsaMotion_PlayerNeo@ha\n"
	        "   slwi r0, r0, 2\n"
	        "   addi r3, r3, lbl_001F1140@l\n"
	        "   addi r18, r4, tu16ExclusiveMotionNo@l\n"
	        "   lwzx r22, r3, r0\n"
	        "   lhz r0, 0(r18)\n"
	        "   rlwinm r4, r6, 0xb, 0xd, 0x14\n"
	        "   addi r3, r5, gpsaMotion_PlayerNeo@l\n"
	        "   cmplwi r22, 0\n"
	        "   add r3, r3, r4\n"
	        "   slwi r0, r0, 2\n"
	        "   add r23, r3, r0\n"
	        "   beq lbl_00045700\n"

	        "lbl_000456B8:\n"
	        "   lwz r0, 0(r16)\n"
	        "   mr r3, r23\n"
	        "   addi r16, r16, 4\n"
	        "   add r4, r15, r0\n"
	        "   bl utilBinSetUpMotion\n"
	        "   lhz r4, 0(r18)\n"
	        "   addi r17, r17, 1\n"
	        "   addi r23, r23, 4\n"
	        "   addi r0, r4, 1\n"
	        "   sth r0, 0(r18)\n"
	        "   lbz r0, 0(r22)\n"
	        "   slwi r3, r0, 1\n"
	        "   addi r0, r3, 0xc\n"
	        "   sthx r4, r21, r0\n"
	        "   lbz r0, 1(r22)\n"
	        "   addi r22, r22, 1\n"
	        "   cmplwi r0, 0xff\n"
	        "   bne lbl_000456B8\n"

	        "lbl_00045700:\n"
	        "   mr r3, r17\n"
	        "   lmw r14, 0x18(r1)\n"
	        "   lwz r0, 0x64(r1)\n"
	        "   mtlr r0\n"
	        "   addi r1, r1, 0x60\n");
}

SubFont CharacterInformationFont;

ASMUsed void
PackManHandler_DebugMenuText(void *fileStart, const u16 *fileIndexStart, u32 *fileOffsetStart, u32 currentFolderIndex) {
	// ID: 48401
	Text2dFileData *textData = &DebugMenu_TextData;
	u16 index = *(u16 *) ((u8 *) fileIndexStart + currentFolderIndex);

	PackMan_SetupRenderedText(fileStart, fileOffsetStart + index, &textData->textData[0]);
	PackMan_SetupRenderedText(fileStart, fileOffsetStart + (index + 1), &textData->extraTextData[0]);
	PackMan_SetupRenderedText(fileStart, fileOffsetStart + (index + 2), &CustomMusicTextData.textDataHeader);
    PackMan_SetupRenderedText(fileStart, fileOffsetStart + (index + 3), &CharacterInformationFont.textDataHeader);
}

std::array<std::array<void*, MAX_SKIN_COUNT>, TotalCharacterAmount> CharacterSkinTextures;

ASMUsed void PackManHandler_CharacterSkins(PackManFileHeader *fileStart,
                                       const u16 *fileIndexStart,
                                       u32 *fileOffsetStart,
                                       u32 currentFolderIndex,
                                       u32 playerIndex) {
	// ID: 4
	u16 index = *(u16 *) ((u8 *) fileIndexStart + currentFolderIndex);
	u32 skinCount = fileStart->fileCounts[0]; // archive only contains one folder, so this will take the texture archive count in first folder
	const Player& player = players[playerIndex];

	for (u32 i = 0; i < skinCount; i++) {
		SetupGVRTextureArchive(fileStart, fileOffsetStart + (index + i), &CharacterSkinTextures[player.character][i], 0);
	}

}

ASMUsed void PackManHandler_BoardWithNoTextures(PackManFileHeader *fileStart,
                                                const u16 *fileIndexStart,
                                                u32 *fileOffsetStart,
                                                u32 currentFolderIndex,
                                                u32 playerIndex) {
    // ID: 5

    const u16 index = *(u16 *) ((u8 *) fileIndexStart + currentFolderIndex);

    // getset_gno
    lbl_000478A0(fileStart, fileOffsetStart + index, &bss_BoardOnlyModelData[playerIndex]);
}

m2darray<void*, MaxPlayerCount, 3> EggmeisterHeap;

ASMUsed void PackManHandler_EggmeisterTextures(PackManFileHeader *fileStart,
                                               const u16 *fileIndexStart,
                                               u32 *fileOffsetStart,
                                               u32 currentFolderIndex,
                                               u32 playerIndex) {
    // ID: 6

    const u16 index = *(u16 *) ((u8 *) fileIndexStart + currentFolderIndex);
    const Player& player = players[playerIndex];

    if (player.extremeGear >= ExtremeGear::ERider && player.extremeGear <= ExtremeGear::SuperHangOn) {
        throw std::logic_error("Bike found in Eggmeister PackManHandler!");
    }

    // get pointer to gvr texture archive
    const auto *gvrArchive = reinterpret_cast<const GVRTextureArchive *>(
            reinterpret_cast<u8 *>(fileStart) + *(fileOffsetStart + index)
    );

    u32 textureIndex;

    // get diffuse texture index
    // index 0 is the reflection texture
    auto exLoads = FetchEnabledEXLoadIDs(player);
    if (exLoads.isExloadEnabled() && exLoads.gearExLoadID == StardustSpeederEXLoad) {
        textureIndex = 1;
    } else {
        textureIndex = GetEggmeisterTextureIndexInArchive(&player) + 1;
    }

    if (textureIndex >= gvrArchive->texNum) {
        throw std::length_error("Eggmeister texture index calculation failed! Texture archive too small!");
    }

    // i deeply apologize for this next confusing pointer arithmetic bit

    // get pointer to the reflection texture, it's the first texture in the archive
    const auto *refGvrTexture = reinterpret_cast<const GVRTexture *>(
            reinterpret_cast<const u8 *>(gvrArchive) + gvrArchive->offsetTable[0]
            );

    // get pointer to gear diffuse texture, determined by the gear you're riding
    const auto *gvrTexture = reinterpret_cast<const GVRTexture *>(
            reinterpret_cast<const u8 *>(gvrArchive) + gvrArchive->offsetTable[textureIndex]
            );

    const u32 refTexSize = convertLittleToBig(refGvrTexture->texSize) + 0x18;
    const u32 texSize = convertLittleToBig(gvrTexture->texSize) + 0x18;

    // set up texture struct
    auto *texStructHeader = static_cast<TexStructHeader*>(gNp_MallocHi(4, func_80053E38(2)));

    func_80053D20(&bss_BoardOnlyTextures[playerIndex], 2, texStructHeader);

    void *copiedRefTexture = gNp_MallocHi(32, refTexSize); // textures need to be strictly aligned
    void *copiedTexture = gNp_MallocHi(32, texSize); // textures need to be strictly aligned

    // copy textures over to new location
    memcpy(copiedRefTexture, refGvrTexture, refTexSize);
    memcpy(copiedTexture, gvrTexture, texSize);

    // save to custom heap array, so can free them from elsewhere later
    EggmeisterHeap[playerIndex][0] = copiedRefTexture;
    EggmeisterHeap[playerIndex][1] = copiedTexture;
    EggmeisterHeap[playerIndex][2] = texStructHeader;

    InitTextureStruct(texStructHeader->texStart, &defaultTexInfo, copiedRefTexture);
    InitTextureStruct(texStructHeader->texStart + 1, &defaultTexInfo, copiedTexture);

    // free the file, it's too large and no longer necessary
    // in the CSS, the file is freed when ready through CheckIfCanFreeEggmeisterCSS(), cuz we cant free it right away

    const bool inCss = CurrentGameMode == TitleScreen;
    if (!inCss) { nnFree(fileStart); }
}

ASMUsed void CheckIfCanFreeEggmeisterCSS(u32 index) {
    CSSModel &eggData = cssEggmeisterTextures[index];

    if (eggData.file != nullptr && eggData.status == 5) {
        CSS_FreeHigh(eggData.count);
        eggData.status = 0;
    }
}

ASMUsed void FreeAllEggmeisterFilesCSS() {
    for (u32 i = 0; i < MaxPlayerCount; i++) {
        FreeEggmeisterHeap(i);
    }
}