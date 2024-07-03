#pragma once

#include "list.hpp"
#include "_thread.hpp"

class _sem {

    unsigned init;
    List<_thread> blockedThreads;

    _sem(unsigned int init) : init(init) {}

public:
    static _sem* open(unsigned init);
    static int close(_sem* handle);
    static int wait(_sem* handle);
    static int signal(_sem* handle);
};