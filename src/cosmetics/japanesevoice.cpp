#include "lib/stdlib.hpp"
#include "lib/thread.hpp"
#include "lib/sound.hpp"
#include "handlers/files/filehandler_dat.hpp"
//#include "handlers/files/filehandler_dat.hpp"

//ASMDefined char str_VOICE_E_AFS;
//ASMDefined char str_VOICE_AFS;
//ASMDefined void lbl_8009E734(u32, void*, u32, void*);
//ASMDefined u32 lbl_8009E398(u32);
//ASMDefined void* lbl_10026720;

//#define ThreadClassTesting

#ifndef ThreadClassTesting
void *JapaneseVoices_Thread;
#endif
#ifdef ThreadClassTesting
std::unique_ptr<Thread> JapaneseVoices_Thread;
#endif

void JapaneseVoiceToggleHandler() {
	auto oldLanguage = GameLanguage;
    if ((RuleSettings & 0x100) != 0) {
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
	static constexpr auto endOfStack = stackOffset+stackSize;
    void *thread = aligned_malloc(32, sizeof(Thread));
   // void *threadMain = (u8*)thread + 1000;
    JapaneseVoices_Thread = thread;
    OSCreateThread(thread, &JapaneseVoiceToggleHandler, 0, (u8*)thread+endOfStack, stackSize, 31, 1);
    OSResumeThread(thread);
#endif
}