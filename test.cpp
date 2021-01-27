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
        printf("%d\n", start + i);
        // 切出当前协程
        S.coroutine_yield();
    }
}

void foo2(Schedule &Sch, void *args){
    auto *S = new Schedule;

    struct args arg1 = {20};
    struct args arg2 = {200};

    int co1 = S->coroutine_create((co_func)foo, &arg1);
    int co2 = S->coroutine_create((co_func)foo, &arg2);

    int i = 0;
    printf("\nmain2 start\n");
    while (S->coroutine_status(co1) || S->coroutine_status(co2)) {
        // use coroutine co1 of main2
        if (S->coroutine_status(co1)){
            printf("main2: ");
            S->coroutine_resume(co1);
        }
        // use coroutine co2 of main2
        if (S->coroutine_status(co2)) {
            printf("main2: ");
            S->coroutine_resume(co2);
        }

        if(i++ == 2){
            S->coroutine_destroy(co2);
        }
    }
    printf("main2 end\n");
}

void test(Schedule *S){
    struct args arg1 = {10};
    struct args arg2 = {100};

    int co1 = S->coroutine_create((co_func)foo, &arg1);
    int co2 = S->coroutine_create((co_func)foo2, &arg2, INDEPENDENT_STACK, 10240);
    //int co2 = S->coroutine_create((co_func)foo, &arg2, INDEPENDENT_STACK, 1024);

    int i = 0;
    printf("main1 start\n");
    while (S->coroutine_status(co1) || S->coroutine_status(co2)) {
        // use coroutine co1 of main1
        if (S->coroutine_status(co1)){
            printf("main1: ");
            S->coroutine_resume(co1);
        }

        // use coroutine co2 of main1
        if (S->coroutine_status(co2)) {
            printf("main1: ");
            S->coroutine_resume(co2);
        }

        if(i++ == 2){
            S->coroutine_destroy(co1);
            //printf("Terminate main1");
            //delete S;
            //break;
        }
    }
    printf("main1 end\n");
}

int main() {
    auto *S = new Schedule;

    test(S);

    if (!S)
        delete S;

    return 0;
}
