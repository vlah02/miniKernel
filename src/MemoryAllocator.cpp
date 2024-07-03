#include "../h/MemoryAllocator.hpp"


void MemoryAllocator::init() {
    if (freeMemHead== nullptr){
        freeMemHead=(FreeMemBlock*)((uint64*)HEAP_START_ADDR);
        freeMemHead->next= nullptr;
        freeMemHead->prev= nullptr;
        freeMemHead->size=((uint64)((uint64*)HEAP_END_ADDR-(uint64*)HEAP_START_ADDR)-sizeof(MemoryAllocator));
        freeMemHead->size/=MEM_BLOCK_SIZE;
    }
}

void *MemoryAllocator::mem_alloc(size_t size) {
    this->init();

    void* ret= nullptr;
    FreeMemBlock* curr;
    bool flag= false;
    for (curr=freeMemHead;curr;curr=curr->next){
        if (curr->size>=size) {
            flag=true;
            break;
        }
    }

    if (!flag) return nullptr;
    ret=curr;
    if (curr->size<=size+sizeof(FreeMemBlock)) {
        if (curr->prev) curr->prev->next = curr->next;
        else freeMemHead = curr->next;

        if (curr->next) curr->next->prev = curr->prev;
        curr->size = size;
    }

    else{
        FreeMemBlock* restFragment=(FreeMemBlock*)((uint64*)curr+size*MEM_BLOCK_SIZE+sizeof (FreeMemBlock));

        if (curr->prev) curr->prev->next=restFragment;
        else freeMemHead=restFragment;

        if (curr->next) curr->next->prev=restFragment;

        restFragment->prev=curr->prev;
        restFragment->next=curr->next;
        restFragment->size=curr->size-size;
        curr->size=size;
    }

    return (uint64*)ret+sizeof(FreeMemBlock);
}

void MemoryAllocator::tryToJoin(FreeMemBlock *block) {
    if (block== nullptr || block->next== nullptr) return;

    if ((char*)block+block->size*MEM_BLOCK_SIZE+sizeof (FreeMemBlock)==(char*)(block->next)){
        block->size+=block->next->size;
        block->next=block->next->next;
        if (block->next) block->next->prev=block;
    }
}

int MemoryAllocator::mem_free(void *ptr) {
    if ((uint64*)ptr > (uint64*)HEAP_END_ADDR || (uint64*)ptr<(uint64*)HEAP_START_ADDR){
        return -1;
    }
    FreeMemBlock* tmp; //prvi block sa manjom adresom u memoriji -> ocuva se sortiranost
    if (!freeMemHead || (char*) ptr<(char*)freeMemHead) tmp= nullptr;
    else{
        for (tmp=freeMemHead;tmp->next!= nullptr && (char*)ptr>(char*)(tmp->next);tmp=tmp->next);
    }

    FreeMemBlock* newBlock=(FreeMemBlock*)((uint64*)ptr-sizeof(FreeMemBlock));
    newBlock->prev=tmp;
    if (tmp) newBlock->next=tmp->next;
    else newBlock->next=freeMemHead;

    if (newBlock->next) newBlock->next->prev=newBlock;
    if (tmp) tmp->next=newBlock;
    else freeMemHead=newBlock;

    tryToJoin(newBlock);
    tryToJoin(tmp);


    return 0;
}

MemoryAllocator &MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}




