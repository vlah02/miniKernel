#include "../h/_thread.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

extern void userMain();
void userMainWrapper(void*) {
    userMain();
}

void dispatchThreads(void*) {
    while(true) {
        thread_dispatch();
    }
}


void main() {

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap|1);

    thread_t main = _thread::createThread(nullptr, nullptr, nullptr);
    _thread::running = main;

    thread_t dispatch;
    thread_create(&dispatch, dispatchThreads, nullptr);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);


    __asm__ volatile ("li a0, 0x99");
    __asm__("ecall");


    thread_t user;
    thread_create(&user, userMainWrapper, nullptr);


    thread_join(user);


}
