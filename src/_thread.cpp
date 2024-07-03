#include "../h/_thread.hpp"
#include "../h/riscv.hpp"
#include "../h/print.hpp"


_thread *_thread::running = nullptr;


_thread *_thread::createThread(Body body, void* arg, uint64* stack) {
    _thread* thread = (_thread*) (new _thread(body, arg, stack));
    return thread;
}



void _thread::yield() {
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}


void _thread::dispatch() {
    _thread *old = running;
    if (running->blocked == 0) {
        //if (!old->isFinished()){
        if (!old->isFinished() && !old->isParentWaiting) {
            Scheduler::put(old);
        }
    }
    running = Scheduler::get();
    _thread::contextSwitch(&old->context, &running->context);
}


void _thread::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);

    if (running->isParentWaiting){
        running->isParentWaiting=false;
        Scheduler::put(running->parent);
    }

    _thread::yield();
}


int _thread::exitThread() {
    if (!running->isFinished()) {
        running->setFinished(true);
        dispatch();
        return 0;
    }
    else
        return -1;
}

void _thread::join(thread_t handle) {

    running->isParentWaiting=true;
    dispatch();

    //yield();

//    while (!handle->isFinished()) {
//        _thread::dispatch();
//    }
}
