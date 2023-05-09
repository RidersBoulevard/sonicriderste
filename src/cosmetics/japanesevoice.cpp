#include "context.hpp"

global char str_VOICE_E_AFS;
global char str_VOICE_AFS;
global void lbl_8009E734(u32, void*, u32, void*);
global u32 lbl_8009E398(u32);
global void* lbl_10026720;
global u8 lbl_1001A6E4;
global u32 GameLanguage;

void *JapaneseVoices_Thread;

void JapaneseVoiceToggleHandler() {
    char *filename;

    if (RuleSettings & 0x100) {
        // japanese voices
        GameLanguage = 0;
        filename = &str_VOICE_AFS;
    } else {
        // english voices
        GameLanguage = 1;
        filename = &str_VOICE_E_AFS;
    }

    lbl_8009E734(0, filename, 0, &lbl_10026720);
    lbl_1001A6E4 += 1;

    do {

    } while (lbl_8009E398(0) != 3);

    nnFree(JapaneseVoices_Thread);
    OSCancelThread(_OSGetCurrentContext());
}

ASMUsed void CreateJapaneseVoiceToggleThread() {
    void *thread = aligned_malloc(32, 10000);
    void *threadMain = (u8 *)thread + 1000;
    JapaneseVoices_Thread = thread;
    OSCreateThread(threadMain, &JapaneseVoiceToggleHandler, 0, (u8*)thread+4000+4096, 4096, 31, 1);
    OSResumeThread(threadMain);
}