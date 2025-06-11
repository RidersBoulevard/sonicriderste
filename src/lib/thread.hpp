#pragma once

#include "main.hpp"

// OS Functions
ASMDefined bool OSCreateThread(void *thread, Task function, u32, void *stackPtr, u32 stackSize, u32, u32);
ASMDefined void OSResumeThread(void *thread);
ASMDefined void OSCancelThread(void *thread);

//What does this return?
ASMDefined void *OSGetCurrentThread();

class alignas(32) Thread {
public:
	//static constexpr auto ThreadSize = 10000;
	static constexpr auto stackSize = 1024;

private:
	//fillerData<1000> unk{};
	std::array<u32, 0x4CA> main{};
	std::array<u32, stackSize> stackSpace{};

public:
	explicit Thread(Task functionPtr);
	void Start();
	void Stop();

	static Thread &GetCurrentThread();
};
//static_assert(sizeof(Thread) == Thread::ThreadSize);