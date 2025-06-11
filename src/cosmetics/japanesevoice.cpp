#include <main.hpp>
#include <lib/sound.hpp>

#include "globalDefs.hpp"
#include "types.hpp"

#include <ogc/lwp.h>
#include "handlers/files/filehandler_dat.hpp"
#include "riders/gamemode.hpp"


//#define ThreadClassTesting

/*#ifndef ThreadClassTesting
void *JapaneseVoices_Thread;
#endif
#ifdef ThreadClassTesting
std::unique_ptr<Thread> JapaneseVoices_Thread;
#endif

void JapaneseVoiceToggleHandler() {
	const auto oldLanguage = GameLanguage;
    if (ruleSettings.japanese_voices) {
        // japanese voices
        GameLanguage = 0;
        filename = &str_VOICE_AFS;
    } else {
        // english voices
        GameLanguage = 1;
        filename = &str_VOICE_E_AFS;
    }

    lbl_8009E734(0, filename, 0, &lbl_10026720) ;
    lbl_1001A6E4 += 1;

    while (lbl_8009E398(0) != 3){}

#ifdef ThreadClassTesting
    JapaneseVoices_Thread->Stop();
    JapaneseVoices_Thread.reset();
#endif
#ifndef ThreadClassTesting
    nnFree(JapaneseVoices_Thread);
    OSCancelThread(OSGetCurrentThread());
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
    static constexpr auto endOfStack = stackOffset + stackSize;
    void *thread = aligned_malloc(32, sizeof(Thread));
    // void *threadMain = (u8*)thread + 1000;
    JapaneseVoices_Thread = thread;
    OSCreateThread(thread, &JapaneseVoiceToggleHandler, nullptr, (u8 *) thread + endOfStack, stackSize, 31,
                   OS_THREAD_ATTR_DETACH);
    OSResumeThread(thread);
#endif
}*/

lwp_t JapaneseVoices_Thread;

[[noreturn]] void* JapaneseVoiceToggleHandler(void* arg) {
	(void)arg; // unused

    while (true) {
        LWP_SuspendThread(LWP_GetSelf());

        auto oldLanguage = GameLanguage;

        if (ruleSettings.japanese_voices) {
            // japanese voices
            GameLanguage = Language::Japanese;
        } else {
            // english voices
            GameLanguage = Language::English;
        }

        if(oldLanguage != GameLanguage){
            FreeDATFiles();
            LoadMainMenuDatFiles();
        }
    }
}

void CreateJapaneseVoiceThread() {
	LWP_CreateThread(&JapaneseVoices_Thread, &JapaneseVoiceToggleHandler, nullptr, nullptr, 0, 8);
}

ASMUsed void ResumeJapaneseVoiceThread() {
    LWP_ResumeThread(JapaneseVoices_Thread);
}