#include "../h/syscall_c.hpp"

void *mem_alloc(size_t size) {
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE));
    __asm__ volatile ("li a0, 0x01");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (void*) ret;

}
int mem_free(void *ptr) {
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (ptr));
    __asm__ volatile ("li a0, 0x02");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (int)ret;

}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    void *stack_space = nullptr;
    if (start_routine)
        stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv a4, %[temp]" : : [temp] "r" (stack_space));
    __asm__ volatile ("mv a3, %[temp]" : : [temp] "r" (arg));
    __asm__ volatile ("mv a2, %[temp]" : : [temp] "r" (start_routine));
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (handle));
    __asm__ volatile ("li a0, 0x11");

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return ret;
}
int thread_exit() {
    __asm__ volatile ("li a0, 0x12");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (int)ret;
}
void thread_dispatch() {
    __asm__ volatile ("li a0, 0x13");

    __asm__ volatile ("ecall");

    return;
}
void thread_join(thread_t handle) {
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (handle));
    __asm__ volatile ("li a0, 0x14");

    __asm__ volatile ("ecall");

    return;
}

int sem_open(sem_t *handle, unsigned int init) {
    __asm__ volatile ("mv a2, %[temp]" : : [temp] "r" (init));
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (handle));
    __asm__ volatile ("li a0, 0x21");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (int)ret;
}
int sem_close(sem_t handle) {
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (handle));
    __asm__ volatile ("li a0, 0x22");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (int)ret;
}
int sem_wait(sem_t id) {
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (id));
    __asm__ volatile ("li a0, 0x23");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (int)ret;
}
int sem_signal(sem_t id) {
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (id));
    __asm__ volatile ("li a0, 0x24");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (int)ret;
}


char getc() {
    __asm__ volatile ("li a0, 0x41");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[temp], a0" : [temp] "=r" (ret));
    return (int)ret;
}
void putc(char c) {
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (c));
    __asm__ volatile ("li a0, 0x42");

    __asm__ volatile ("ecall");

    return;
}

