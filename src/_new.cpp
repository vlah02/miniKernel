#include "../h/syscall_c.hpp"



using size_t = decltype(sizeof(0));


void *operator new(size_t n) {
    return mem_alloc((n + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

void *operator new[](size_t n) {
    return mem_alloc((n + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}


void operator delete(void *p) {
    mem_free(p);
}

void operator delete[](void *p) {
    mem_free(p);
}