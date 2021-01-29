//
// Created by xclwt on 2020/7/23.
//


#ifndef COROUTINE_COROUTINE_H
#define COROUTINE_COROUTINE_H

#include <ucontext.h>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <vector>
#include <iostream>
#include "coctx.h"

#define STACK_SIZE 1024 * 1024
#define COROUTINE_NUM 16

#define COROUTINE_DEAD 0
#define COROUTINE_READY 1
#define COROUTINE_RUNNING 2
#define COROUTINE_SUSPEND 3

#define SAVED_STACK 0
#define INDEPENDENT_STACK 1

using namespace std;

class Schedule;
class Coroutine;

class coctx;

void save_stack(Coroutine *co, const char *top);

#ifdef USE_SYS_UCONTEXT
void start_func(uint32_t low_addr, uint32_t high_addr);
#else
void start_func(void *S);
#endif

typedef void (*co_func)(Schedule &S, void *args);

class Coroutine {
public:
#ifdef USE_SYS_UCONTEXT
    ucontext_t ucontext{};
#else
    coctx ucontext{};
#endif
    int status;
    char* stack;
    /*size of the memory allocated to save-stack*/
    int cap;
    /*size of memory used to save running stack*/
    int size;
    int type;

    Coroutine(co_func func, void *args, int stack_type, int stack_size);

    ~Coroutine();

#ifdef USE_SYS_UCONTEXT
    friend void start_func(uint32_t low_addr, uint32_t high_addr);
#else
    friend void start_func(void *S);
#endif


private:
    co_func co_start;
    void *func_arg;

};


class Schedule {

public:
#ifdef USE_SYS_UCONTEXT
    ucontext_t main{};
#else
    coctx main{};
#endif
    /* default schedule */
    Schedule();

    /* schedule with appointed size of shared runtime-stack */
    Schedule(int stack_size);

    ~Schedule();

    /* create a coroutine with shared runtime-stack */
    int coroutine_create(co_func func, void *args);

    /* create a coroutine with shared runtime-stack */
    int coroutine_create(co_func func, void *args, int stack_type, int stack_size);

    /* destroy a coroutine */
    void coroutine_destroy(int co_id);

    /* resume the coroutine */
    void coroutine_resume(int co_id);

    /* yield the running coroutine */
    void coroutine_yield();

    int coroutine_status(int co_id);

    int coroutine_running() const;

#ifdef USE_SYS_UCONTEXT
    friend void start_func(uint32_t low_addr, uint32_t high_addr);
#else
    friend void start_func(void *S);
#endif


private:
    char* stack;
    int co_num;
    int co_cap;
    int stack_size;
    int running_id;
    vector<Coroutine *> co_list;
};

#endif //COROUTINE_COROUTINE_H
