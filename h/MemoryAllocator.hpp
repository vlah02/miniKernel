#pragma once

#include "../lib/hw.h"


struct FreeMemBlock{
    FreeMemBlock* prev;
    FreeMemBlock* next;
    size_t size;
};

class MemoryAllocator{
public:

    MemoryAllocator(const MemoryAllocator&)=delete;
    MemoryAllocator& operator=(const MemoryAllocator&)=delete;

    static MemoryAllocator& getInstance();
    void* mem_alloc(size_t size);
    int mem_free(void*);

private:
    FreeMemBlock* freeMemHead= nullptr;
    void tryToJoin(FreeMemBlock*);
    MemoryAllocator()=default;
    ~MemoryAllocator()=default;
    void init();

};

