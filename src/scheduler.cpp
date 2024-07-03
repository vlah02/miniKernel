#include "../h/scheduler.hpp"
#include "../h/print.hpp"



List<_thread> Scheduler::readyCoroutineQueue;


_thread *Scheduler::get() {
    _thread* temp = readyCoroutineQueue.removeFirst();
//    printString("GET\n");
//    readyCoroutineQueue.iterate();
//    printString("\n");
    return temp;
}

void Scheduler::put(_thread *ccb) {
    readyCoroutineQueue.addLast(ccb);
//    printString("PUT\n");
//    readyCoroutineQueue.iterate();
//    printString("\n");
}