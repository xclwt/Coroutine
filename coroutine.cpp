//
// Created by xclwt on 2020/7/23.
//

#include "coroutine.h"

Coroutine::Coroutine(co_func func, void *args){
        co_start = func;
        func_arg = args;
        cap = 0;
        size = 0;
        status = COROUTINE_READY;
        stack = nullptr;
    }

Coroutine::~Coroutine(){
        delete[] stack;
    }


Schedule::Schedule(){
        stack = new char[STACK_SIZE];
        co_num = 0;
        co_cap = COROUTINE_NUM;
        running_id = -1;
        co_list = vector<Coroutine *>(COROUTINE_NUM, nullptr);
    }

Schedule::~Schedule(){
        for (int i = 0; i < co_cap; ++i){
            Coroutine *co = co_list[i];

            if (co != nullptr){
                printf("fail%d\n",i);
                co->~Coroutine();
            }
        }

        delete[] stack;
    }

int Schedule::coroutine_create(co_func func, void *args){
        auto *co = new Coroutine(func, args);

        if (co_num == co_cap){
            co_list.push_back(co);
            ++co_num;
            co_cap = co_list.capacity();
            return co_num - 1;
        }else{
            for (int i = 0; i < co_cap; ++i){
                int co_id = (i + co_num) % co_cap;
                if (!co_list[co_id]){
                    co_list[co_id] = co;
                    ++co_num;
                    return co_id;
                }
            }
            return -1;
        }
    }

void Schedule::coroutine_resume(int co_id){
        /*if called in non-main co, teminate*/
        assert(this->running_id == -1);

        /*if called with illegal co_id, terminate*/
        assert(co_id >= 0 && co_id < co_cap);
        Coroutine *co = this->co_list[co_id];

        switch (co->status){
            case COROUTINE_READY: {
                getcontext(&co->ucontext);
                co->ucontext.uc_stack.ss_sp = this->stack;
                co->ucontext.uc_stack.ss_size = STACK_SIZE;
                /*ucontext will change to main after non-main co return*/
                co->ucontext.uc_link = &this->main;
                co->status = COROUTINE_RUNNING;
                this->running_id = co_id;

                auto S_ptr = (uintptr_t) this;
                makecontext(&co->ucontext, (void (*)()) start_func, 2, (uint32_t) S_ptr, (uint32_t) (S_ptr >> 32));
                /*main co to non-main co*/
                swapcontext(&this->main, &co->ucontext);
                break;
            }
            case COROUTINE_SUSPEND:{
                memcpy(this->stack + STACK_SIZE - co->size, co->stack, co->size);
                co->status = COROUTINE_RUNNING;
                this->running_id = co_id;
                swapcontext(&this->main, &co->ucontext);
                break;
            }
        }
    }

void Schedule::coroutine_yield(){
        int co_id = this->running_id;
        Coroutine *co = this->co_list[co_id];
        co->status = COROUTINE_SUSPEND;
        save_stack(co, this->stack + STACK_SIZE);
        this->running_id = -1;

        /*non-main co to main co*/
        swapcontext(&co->ucontext, &this->main);
    }

int Schedule::coroutine_status(int co_id){
        /*if called with illegal co_id, terminate*/
        assert(co_id >= 0 && co_id < co_cap);

        if (this->co_list[co_id] == nullptr)
            return COROUTINE_DEAD;

        return this->co_list[co_id]->status;
    }

int Schedule::coroutine_running() const{
        return this->running_id;
}

void save_stack(Coroutine *co, const char *top){
        char dummy;

        assert(top - &dummy <= STACK_SIZE);

        if(co->cap < top - &dummy){
            co->cap = top - &dummy;
            delete[] co->stack;
            co->stack = new char[co->cap];
        }
        co->size = top - &dummy;
        memcpy(co->stack, &dummy, co->size);
    }


void start_func(uint32_t low_addr, uint32_t high_addr){
    uintptr_t S_ptr = ((uintptr_t)high_addr << 32) + (uintptr_t)low_addr;
    auto *S = (Schedule *)S_ptr;
    Coroutine *co = S->co_list[S->running_id];

    co->co_start(*S, co->func_arg);

    delete co;
    S->co_num--;
    S->co_list[S->running_id] = nullptr;
    S->running_id = -1;
}