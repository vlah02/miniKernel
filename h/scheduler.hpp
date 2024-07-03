#pragma once

#include "list.hpp"


class _thread;


class Scheduler {
private:
    static List<_thread> readyCoroutineQueue;

public:
    static _thread *get();

    static void put(_thread *ccb);

};
