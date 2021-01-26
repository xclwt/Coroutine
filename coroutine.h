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

#define STACK_SIZE 1024 * 1024
#define COROUTINE_NUM 16

#define COROUTINE_DEAD 0
#define COROUTINE_READY 1
#define COROUTINE_RUNNING 2
#define COROUTINE_SUSPEND 3

#define SAVED_STACK 0
#define INDEPENDENT_STACK 1

//#define COROUTINE_OUTPUT

using namespace std;

class Schedule;
class Coroutine;

void save_stack(Coroutine *co, const char *top);

#ifdef COROUTINE_OUTPUT
    void start_func(uint32_t low_addr, uint32_t high_addr, int);
#else
    void start_func(uint32_t low_addr, uint32_t high_addr);
#endif

typedef void (*co_func)(Schedule &S, void *args);

class Coroutine {
public:
    ucontext_t ucontext{};
    int status;
    char* stack;
    /*size of the memory allocated to save-stack*/
    int cap;
    /*size of memory used to save running stack*/
    int size;
    int type;

    Coroutine(co_func func, void *args, int stack_type, int stack_size);

    ~Coroutine();
#ifdef COROUTINE_OUTPUT
    friend void start_func(uint32_t low_addr, uint32_t high_addr, int);
#else
    friend void start_func(uint32_t low_addr, uint32_t high_addr);
#endif

private:
    co_func co_start;
    void *func_arg;

};


class Schedule {

public:
    /* default schedule */
    Schedule();

    /* schedule with appointed size of shared runtime-stack */
    Schedule(int stack_size);

    ~Schedule();

    /* create a coroutine with shared runtime-stack */
    int coroutine_create(co_func func, void *args);

    /* create a coroutine with shared runtime-stack */
    int coroutine_create(co_func func, void *args, int stack_type, int stack_size);

    /* resume the coroutine */
    void coroutine_resume(int co_id);

    /* yield the running coroutine */
    void coroutine_yield();

    int coroutine_status(int co_id);

    int coroutine_running() const;

#ifdef COROUTINE_OUTPUT
    friend void start_func(uint32_t low_addr, uint32_t high_addr, int);
#else
    friend void start_func(uint32_t low_addr, uint32_t high_addr);
#endif

private:
    char* stack;
    ucontext_t main{};
    int co_num;
    int co_cap;
    int stack_size;
    int running_id;
    vector<Coroutine *> co_list;
};




#endif //COROUTINE_COROUTINE_H
