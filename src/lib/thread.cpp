#include "thread.hpp"

Thread::Thread(Task functionPtr) {
	const bool successful = OSCreateThread(this, functionPtr, 0, stackSpace.end(), stackSize, 31, 1);
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
