#include "../h/syscall_cpp.hpp"
#include "../h/riscv.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle = nullptr;
}
Thread::~Thread() {
    myHandle->setFinished(true);
    delete myHandle;
}
int Thread::start() {
    thread_create(&myHandle, &threadWrapper, (void*)this);
    return 0;
}
void Thread::join() {
    thread_join(myHandle);
}
void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    myHandle = nullptr;
}
void Thread::threadWrapper(void *thread) {
    ((Thread*)thread) -> run();
}




Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}
Semaphore::~Semaphore() {
    sem_close(myHandle);
}
int Semaphore::wait() {
    return sem_wait(myHandle);
}
int Semaphore::signal() {
    return sem_signal(myHandle);
}


//void PeriodicThread::terminate() {
//    thread_exit();
//}
//PeriodicThread::PeriodicThread(time_t period) : Thread(), period(period) {
//
//}


char Console::getc() {
    return ::getc();
}
void Console::putc(char c) {
    ::putc(c);
}