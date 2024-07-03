#pragma once


#include "syscall_c.hpp"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();

protected:
    Thread ();
    virtual void run () {}
    static void threadWrapper(void*);
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};
