#include "context.hpp"

// perfect match

struct DATFileBSS {
    u32 ints1[0x20];
    u32 ints2[0x20];
};

global u8 lbl_0014C32C();
global void lbl_8008812C(void *);
global u32 lbl_8008D9F8(u32);
global void lbl_8008E2FC(u32, void *, void *, u32, u32, u32);
global DATFile lbl_001E9CE0;
global DATFile lbl_001E9EBC;
global s8 lbl_1001A6E4;
global DATFileBSS lbl_10026620;
global struct lbl_801B2D98 lbl_801B2D98; // this struct is never defined
global s32 GameLanguage;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-magic-numbers"
struct DATFile EN_CharacterVoiceFiles[TotalCharacterAmount] = {
    {"10SONIC.DAT", 0x6C0, 0x214C0, nullptr},
    {"10TAILS.DAT", 0x7E0, 0x2D5C0, nullptr},
    {"10KNUCK.DAT", 0x760, 0x24BA0, nullptr},
    {"10AMY.DAT", 0x7E0, 0x24280, nullptr},
    {"10JET.DAT", 0x7E0, 0x279C0, nullptr},
    {"10STORM.DAT", 0x7E0, 0x236A0, nullptr},
    {"10WAVE.DAT", 0x7E0, 0x1F180, nullptr},
    {"10EGGM.DAT", 0x7E0, 0x38920, nullptr},
    {"10CREAM.DAT", 0x760, 0x2A960, nullptr},
    {"10ROUGE.DAT", 0x760, 0x237A0, nullptr},
    {"10SHADO.DAT", 0x7E0, 0x2D960, nullptr},
    {"10SS.DAT", 0x6C0, 0x1A6C0, nullptr},
    {"10NIGHT.DAT", 0xF20, 0x124240, nullptr},
    {"10AIAI.DAT", 0xA20, 0x2DF20, nullptr},
    {"10ULALA.DAT", 0x900, 0x24360, nullptr},
    {"10ER1.DAT", 0x520, 0x19740, nullptr},
    {"10ER2.DAT", 0x640, 0x18200, nullptr},
    {"10SILV.DAT", 0x900, 0x3CB20, nullptr},
    {"10METAL.DAT", 0xE80, 0xB0700, nullptr},
    {"10EMERL.DAT", 0x760, 0x2BC00, nullptr},
    {"10BLAZE.DAT", 0x860, 0x3DD00, nullptr},
    {"10CHAOS.DAT", 0x760, 0x43F20, nullptr},
    {"10TIKAL.DAT", 0x900, 0x4AB60, nullptr},
};

struct DATFile JP_CharacterVoiceFiles[TotalCharacterAmount] = {
    {"10JSONIC.DAT", 0x6C0, 0x1B6E0, nullptr},
    {"10JTAILS.DAT", 0x760, 0x1C9C0, nullptr},
    {"10JKNUCK.DAT", 0x760, 0x1A540, nullptr},
    {"10JAMY.DAT", 0x7E0, 0x2B540, nullptr},
    {"10JJET.DAT", 0x7E0, 0x3A9C0, nullptr},
    {"10JSTORM.DAT", 0x7E0, 0x3A060, nullptr},
    {"10JWAVE.DAT", 0x7E0, 0x206A0, nullptr},
    {"10JEGGM.DAT", 0x7E0, 0x3DEE0, nullptr},
    {"10JCREAM.DAT", 0x7E0, 0x3D700, nullptr},
    {"10JROUGE.DAT", 0x760, 0x244C0, nullptr},
    {"10JSHADO.DAT", 0x7E0, 0x26720, nullptr},
    {"10JSS.DAT", 0x640, 0x18B00, nullptr},
    {"10NIGHT.DAT", 0xF20, 0x124240, nullptr},
    {"10JAIAI.DAT", 0x7E0, 0x21900, nullptr},
    {"10JULALA.DAT", 0x6C0, 0x17580, nullptr},
    {"10JER1.DAT", 0x520, 0xF360, nullptr},
    {"10JER2.DAT", 0x520, 0x11F40, nullptr},
    {"10SILV.DAT", 0x900, 0x3CB20, nullptr},
	{"10METAL.DAT", 0xE80, 0xB0700, nullptr},
	{"10EMERL.DAT", 0x760, 0x2BC00, nullptr},
    {"10BLAZE.DAT", 0x860, 0x3DD00, nullptr},
	{"10CHAOS.DAT", 0x760, 0x43F20, nullptr},
    {"10TIKAL.DAT", 0x900, 0x4AB60, nullptr},
};

// custom data

DATFile E10Dats[2] = {
    {"10EB2.DAT", 0x640, 0x1F340, nullptr},
    {"10ER2.DAT", 0x520, 0x18200, nullptr}
};
#pragma clang diagnostic pop

ASMUsed void LoadCharacterDATFile() {
    u32 var_r30;
    Player *var_r29;
    DATFile *var_r31;
    s32 temp_r26;
    s32 temp_r27;
    s32 temp_r27_2;
    s8 temp_r0;
    void* temp_r25;
    
    u32 temp_r5;
    DATFile *temp_r24;
    DATFileBSS *temp_r4;

    var_r30 = 0U;
    
    if (GameLanguage == 0) {
        var_r31 = &JP_CharacterVoiceFiles[0];
    } else {
        var_r31 = &EN_CharacterVoiceFiles[0];
    }
    var_r29 = players.data();
    TRK_memset(&lbl_801B2D98, 0x7F, 0xFF);
    do {
        temp_r0 = static_cast<s8>(var_r29->character);
        if (static_cast<u32>(temp_r0) < TotalCharacterAmount) {
            // if ((u32) temp_r0 > 0x10) temp_r0 = 0; // this is so it'll default to sonic for new characters for now
            if (temp_r0 == E10R) {
                temp_r24 = &E10Dats[0];
            } else {
                temp_r24 = var_r31 + temp_r0;
            }
            
            temp_r27 = static_cast<s32>(temp_r24->spec2);
            temp_r26 = static_cast<s32>(temp_r24->spec1);
            temp_r25 = aligned_malloc(0x20, 0x20000);
            temp_r5 = static_cast<u32>(static_cast<u8>(lbl_1001A6E4));
            temp_r4 = &lbl_10026620;
            temp_r4->ints1[temp_r5] = static_cast<u32>(temp_r26);
            temp_r4->ints2[temp_r5] = static_cast<u32>(temp_r27);
            lbl_8008812C(&lbl_10026620);
            lbl_8008E2FC(static_cast<u32>(lbl_1001A6E4), temp_r24, temp_r25, 0x20000, 0, 0);
loop_5:
            temp_r27_2 = static_cast<s32>(lbl_8008D9F8(static_cast<u32>(lbl_1001A6E4)));
            if (lbl_0014C32C() != 0) {
                if (temp_r27_2 != 2) {
                    goto loop_5;
                }
            }
            if (temp_r25) {
                nnFree(temp_r25);
            }
            lbl_1001A6E4 += 1;
        }
        var_r30 += 1;
        var_r29 += 1;
    } while (var_r30 < InGamePlayerCount);
}