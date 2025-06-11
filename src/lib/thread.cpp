#include "thread.hpp"

/*
Thread::Thread(ThreadFunc functionPtr) {
	const bool successful = OSCreateThread(this, functionPtr, nullptr, stackSpace.end(), stackSize, 31, OS_THREAD_ATTR_DETACH);
	// Todo: Should we check to make sure creating the thread actually worked?
}
void Thread::Start() {
	OSResumeThread(this);
}
void Thread::Stop() {
	OSCancelThread(this);
	//throw std::runtime_error("Attempting to stop incorrect thread");
}
Thread &Thread::GetCurrentThread() {
	return *static_cast<Thread *>(OSGetCurrentThread());
}
*/
