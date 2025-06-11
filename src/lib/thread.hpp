#pragma once

#include "main.hpp"

/*class alignas(32) Thread {
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
};*/
//static_assert(sizeof(Thread) == Thread::ThreadSize);