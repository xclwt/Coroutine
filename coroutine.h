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

#define STACK_SIZE 1024 * 1024
#define COROUTINE_NUM 16

#define COROUTINE_DEAD 0
#define COROUTINE_READY 1
#define COROUTINE_RUNNING 2
#define COROUTINE_SUSPEND 3

using namespace std;

class Schedule;
class Coroutine;

void start_func(uint32_t low_addr, uint32_t high_addr);
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

    Coroutine(Schedule *S, co_func func, void *args);

    ~Coroutine();

    friend void start_func(uint32_t low_addr, uint32_t high_addr);

private:
    co_func co_start;
    void *func_arg;

};


class Schedule {

public:
    Schedule();

    ~Schedule();

    int coroutine_create(co_func func, void *args);

    void coroutine_resume(int co_id);

    void coroutine_yield();

    int coroutine_status(int co_id);

    int coroutine_running();

    void save_stack(Coroutine *co, const char *top);

    friend void start_func(uint32_t low_addr, uint32_t high_addr);

private:
    char* stack;
    ucontext_t main{};
    int co_num;
    int co_cap;
    int running_id;
    vector<Coroutine *> co_list;
};




#endif //COROUTINE_COROUTINE_H
