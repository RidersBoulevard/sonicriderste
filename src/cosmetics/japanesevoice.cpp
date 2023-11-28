#include "lib/stdlib.hpp"
#include "lib/thread.hpp"

ASMDefined char str_VOICE_E_AFS;
ASMDefined char str_VOICE_AFS;
ASMDefined void lbl_8009E734(u32, void*, u32, void*);
ASMDefined u32 lbl_8009E398(u32);
ASMDefined void* lbl_10026720;
ASMDefined u8 lbl_1001A6E4;
ASMDefined u32 GameLanguage;

//#define ThreadClassTesting

#ifndef ThreadClassTesting
void *JapaneseVoices_Thread;
#endif
#ifdef ThreadClassTesting
std::unique_ptr<Thread> JapaneseVoices_Thread;
#endif

void JapaneseVoiceToggleHandler() {
    char *filename;

    if ((RuleSettings & 0x100) != 0) {
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

    while (lbl_8009E398(0) != 3){}

#ifdef ThreadClassTesting
	JapaneseVoices_Thread->Stop();
	JapaneseVoices_Thread.reset();
#endif
#ifndef ThreadClassTesting
	nnFree(JapaneseVoices_Thread);
    OSCancelThread(_OSGetCurrentContext());
#endif
}

ASMUsed void CreateJapaneseVoiceToggleThread() {
#ifdef ThreadClassTesting
	JapaneseVoices_Thread = std::make_unique<Thread>(JapaneseVoiceToggleHandler);
	JapaneseVoices_Thread->Start();
#endif
#ifndef ThreadClassTesting
	static constexpr auto stackOffset = 3000;
	static constexpr auto stackSize = 4096;
	static constexpr auto endOfStack = stackOffset+stackSize;
    void *thread = aligned_malloc(32, sizeof(Thread));
   // void *threadMain = (u8*)thread + 1000;
    JapaneseVoices_Thread = thread;
    OSCreateThread(thread, &JapaneseVoiceToggleHandler, 0, (u8*)thread+endOfStack, stackSize, 31, 1);
    OSResumeThread(thread);
#endif
}