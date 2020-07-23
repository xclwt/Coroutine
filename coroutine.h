//
// Created by xclwt on 2020/7/23.
//

#ifndef COROUTINE_COROUTINE_H
#define COROUTINE_COROUTINE_H

#include <ucontext.h>
#include <string.h>
#include <assert.h>
#include <vector>

#define STACK_SIZE 1024 * 1024
#define COROUTINE_NUM 16

#define COROUTINE_DEAD 0
#define COROUTINE_READY 1
#define COROUTINE_RUNNING 2
#define COROUTINE_SUSPEND 3

using namespace std;

class Schdule;

typedef void (*co_func)(Schdule &S, void *args);

class Coroutine {
public:
    Coroutine(Schdule *S, co_func func, void *args){
        co_start = func;
        func_arg = args;
        status = COROUTINE_READY;
        stack = NULL;
    }

    ~Coroutine(){

    }



private:
    co_func co_start;
    void *func_arg;
    ucontext_t ucontext;
    int status;
    char *stack;

};


class Schdule {

public:
    Schdule(){
        stack = new char[STACK_SIZE];
        co_num = 0;
        co_cap = COROUTINE_NUM;
        running_id = -1;
        co_list = vector<Coroutine *>(COROUTINE_NUM, NULL);
        //co_list = new Coroutine* [COROUTINE_NUM];
        //co_list = std::vector<Coroutine *>(COROUTINE_NUM);
        //memset(co_list, 0, sizeof(Coroutine*) * COROUTINE_NUM);
    }

    ~Schdule(){
        for (int i = 0; i < co_cap; ++i){
            Coroutine *co = co_list[i];

            if (co){
                co->~Coroutine();
            }
        }

        delete[] stack;
        //delete[] co_list;
        //co_list = NULL;
    }

    int coroutine_create(co_func func, void *args){
        Coroutine co(this, func, args);

        if (co_num == co_cap){
            co_list.push_back(&co);
            ++co_num;
            co_cap = co_list.capacity();
        }else{
            for (int i = 0; i < co_cap; ++i){
                int co_id = (i + co_num) % co_cap;
                if (!co_list[co_id]){
                    co_list[co_id] = &co;
                    ++co_num;
                    return co_id;
                }
            }
        }
    }

    void coroutine_resume(int co_id){
        /*if called in non-main co, teminate*/
        assert(this->running_id == -1);

        /*if called with illegal co_id, terminate*/
        assert(co_id >= 0 && co_id < co_cap);
    }

private:
    char* stack;
    ucontext_t main;
    int co_num;
    int co_cap;
    int running_id;
    vector<Coroutine *> co_list;
};


#endif //COROUTINE_COROUTINE_H
