//
// Created by xclwt on 2020/7/23.
//

#include "coroutine.h"

Coroutine::Coroutine(co_func func, void *args, int stack_type, int stack_size){
    co_start = func;
    func_arg = args;
    cap = stack_size;
    size = 0;
    status = COROUTINE_READY;
    type = stack_type;

    if (type == SAVED_STACK)
        stack = nullptr;
    else if (type == INDEPENDENT_STACK)
        stack = new char[cap];
    else{
        cerr << "Coroutine stack type error." << endl;
        assert(0);
    }

}

Coroutine::~Coroutine(){
    delete[] stack;
    printf("delete successfully.\n");
}
