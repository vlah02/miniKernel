#pragma once

#include "../lib/hw.h"
//#include "../lib/mem.h"
#include "scheduler.hpp"
#include "syscall_c.hpp"
#include "list.hpp"

class _sem;

class _thread {
public:
    ~_thread() {
        mem_free(stack);
    }

    bool isFinished() const {
        return finished;
    }

    void setFinished(bool value) {
        finished = value;
    }

    using Body = void (*)(void*);

    static _thread *createThread(Body body, void* arg, uint64* stack_space);

    static void yield();

    static _thread *running;


    struct Context {
        uint64 ra;
        uint64 sp;
    };

    void* arg;
    Body body;
    uint64* stack;
    Context context;
    bool finished;
    int blocked;

    _thread* parent;
    bool isParentWaiting;

private:


    explicit _thread(Body body, void* arg, void* stack) :
            arg(arg),
            body(body),
            stack((uint64*)stack),
            context({(uint64) &threadWrapper,
                     this->stack != nullptr ? (uint64) &this->stack[2*DEFAULT_STACK_SIZE] : 0
                    }),
            finished(false),
            blocked(0),
            parent(running),
            isParentWaiting(false)
    {
        if (body != nullptr)
            Scheduler::put(this);
    }

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static int exitThread();

    static void join(thread_t handle);
};

