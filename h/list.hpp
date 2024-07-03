#pragma once

#include "../h/print.hpp"
#include "../h/MemoryAllocator.hpp"

template<typename T>
class List {
private:
    struct Elem {
        T *data;
        Elem *next;

        void* operator new(uint64  sz){ return MemoryAllocator::getInstance().mem_alloc(sz);};
        void* operator new[](uint64 sz){ return MemoryAllocator::getInstance().mem_alloc(sz);};
        void operator delete (void* l){  MemoryAllocator::getInstance().mem_free(l);};
        void operator delete[](void* l){ MemoryAllocator::getInstance().mem_free(l);};

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;

public:
    List() : head(0), tail(0) {}


    List(const List<T> &) = delete;
    List<T> &operator=(const List<T> &) = delete;

    void iterate() {
        Elem *elem = head;
        printStr("Scheduler:\n");
        while (elem) {
            printInteger((uint64) elem->data);
            printStr("\n");
            elem = elem->next;
        }
    }

    T *removeFirst() {
        if (!head)
            return 0;

        Elem *elem = head;
        head = head->next;
        if (!head)
            tail = 0;

        T *ret = elem->data;
//        delete elem;
        MemoryAllocator::getInstance().mem_free(elem);
        return ret;
    }

    void addLast(T *data) {
//        Elem *elem = new Elem(data, 0);
        Elem *elem = (Elem*)MemoryAllocator::getInstance().mem_alloc((sizeof(Elem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
        elem->data = data;
        elem->next = 0;
        if (tail) {
            tail->next = elem;
            tail = elem;
        }
        else {
            head = tail = elem;
        }
    }



    T *peekFirst() {
        if (!head)
            return 0;
        return head->data;
    }

    void addFirst(T *data) {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    T *removeLast() {
        if (!head)
            return 0;

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
            prev = curr;

        Elem *elem = tail;
        if (prev)
            prev->next = 0;
        else
            head = 0;
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast() {
        if (!tail)
            return 0;
        return tail->data;
    }
};
