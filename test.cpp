#include <iostream>
#include "coroutine.h"

struct args {
    int n;
};

void foo(Schedule &S, void *args){
    auto *arg = (struct args *)args;
    int start = arg->n;
    int i;
    for (i=0;i<5;i++) {
        printf("coroutine %d : %d\n", S.coroutine_running() , start + i);
        // 切出当前协程
        S.coroutine_yield();
    }
}

void test(Schedule &S){
    struct args arg1 = {10};
    struct args arg2 = {100};

    int co1 = S.coroutine_create((co_func)foo, &arg1);
    int co2 = S.coroutine_create((co_func)foo, &arg2);
    printf("%d\n%d\n",co1,co2);

    printf("main start\n");
    while (S.coroutine_status(co1) && S.coroutine_status(co2)) {
        // 使用协程co1
        S.coroutine_resume(co1);
        // 使用协程co2
        S.coroutine_resume(co2);
    }
    printf("main end\n");
}

int main() {
    Schedule S;

    test(S);
    S.~Schedule();
    printf("1\n");
    return 0;
}
