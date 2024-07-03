#include "../h/_sem.hpp"

_sem* _sem::open(unsigned init) {
    _sem* sem = (_sem*)(new _sem(init));
    return sem;
}

int _sem::wait(_sem* handle) {
    if(handle == nullptr)
        return -1;

    if((int)--(handle->init) < 0) {
        _thread::running->blocked = 1;
        handle->blockedThreads.addLast(_thread::running);
        thread_dispatch();
    }
    return 0;
}

int _sem::signal(_sem* handle) {
    if(handle == nullptr)
        return -1;

    if((int)++(handle->init) <= 0) {
        _thread* thread = handle->blockedThreads.removeFirst();
        thread->blocked = 0;
        Scheduler::put(thread);
    }
    return 0;
}

int _sem::close(_sem *handle) {
    if(!handle)
        return -1;

    while(handle->blockedThreads.peekFirst()) {
        _thread* thread = handle->blockedThreads.removeFirst();
        thread->blocked = 0;
        Scheduler::put(thread);
    }

    return 0;
}


