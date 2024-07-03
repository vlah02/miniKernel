#include "../h/riscv.hpp"

//_buffer* Riscv::bufferGet = nullptr;
//_buffer* Riscv::bufferPut = nullptr;


void Riscv::popSppSpie()
{
    __asm__ volatile("csrc sip, 0x2");
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{

    uint64 volatile a4 = r_a4();
    uint64 volatile a3 = r_a3();
    uint64 volatile a2 = r_a2();
    uint64 volatile a1 = r_a1();
    uint64 volatile a0 = r_a0();

    uint64 volatile scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        if (a0 == 0x01) { // void* mem_alloc (size_t size)
            void *ret = MemoryAllocator::getInstance().mem_alloc((size_t) a1);
            //void *ret = __mem_alloc((size_t) a1);
            w_a0_stack((uint64)ret);
        }
        else if (a0 == 0x02) { // int mem_free(void *ptr)
            int ret = MemoryAllocator::getInstance().mem_free((void*) a1);
            //int ret = __mem_free((void*) a1);
            w_a0_stack((uint64)ret);

        }

        else if (a0 == 0x11) { // int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg)
            _thread** handle = (_thread**)a1;
            *handle = _thread::createThread((_thread::Body)a2, (void*)a3, (uint64*)a4);

            int ret = 0;
            if (*handle == nullptr)
                ret = -1;
            w_a0_stack(ret);
        }
        else if (a0 == 0x12) { // int thread_exit ()
            a0 = _thread::exitThread();
            w_a0_stack(a0);
        }
        else if (a0 == 0x13) { // void thread_dispatch()
            _thread::dispatch();
        }
        else if(a0 == 0x14) { // void thread_join (thread_t handle);
            thread_t handle = (thread_t)a1;
            _thread::join(handle);
        }

        else if (a0 == 0x21) { // int sem_open(sem_t* handle, unsigned init)
            _sem** handle = (_sem**)a1;
            *handle = _sem::open((unsigned)a2);

            int ret = 0;
            if (*handle == nullptr)
                ret = -1;
            w_a0_stack(ret);
        }
        else if (a0 == 0x22) { // int sem_close (sem_t handle);
            a0 = _sem::close((_sem*)a1);
            w_a0_stack(a0);
        }
        else if (a0 == 0x23) { // int sem_wait (sem_t id);
            a0 = _sem::wait((_sem*)a1);
            w_a0_stack(a0);
        }
        else if (a0 == 0x24) { // int sem_signal (sem_t id);
            a0 = _sem::signal((_sem*)a1);
            w_a0_stack(a0);
        }

        else if (a0 == 0x41) { // char getc ();
            //a0 = bufferGet->get();
            a0 = __getc();
            w_a0_stack(a0);
        }
        else if (a0 == 0x42) { // void putc (char);
            char c = (char)a1;
            __putc(c);
            //bufferPut->put(c);
        }

        else if (a0 == 0x99) { //void switchUser();
            uint64 temp = r_sstatus();
            temp &= ~(1<<8);
            sstatus = temp;
        }


        //_thread::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000009UL)
    {
        console_handler();
    }
    else
    {
        // unexpected trap cause
        printInteger(scause);
        printStr("\n");
        printInteger(r_sepc());
        printStr("\n");
    }
}