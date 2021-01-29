//
// Created by xclwt on 2021/1/28.
//

#ifndef COROUTINE_COCTX_H
#define COROUTINE_COCTX_H

#include <cstddef>
#include <cstdint>
#include <bits/types/stack_t.h>

//callee-preserved reg
#define R15 0
#define R14 1
#define R13 2
#define R12 3
#define RBP 4

//argument
#define RDI 5

//ret addr
#define RET 6

//callee-preserved reg
#define RBX 7
#define RSP 8

typedef void (*co_start)(void *s);

typedef struct coctx{
    void *regs[9];
    struct coctx *uc_link;
    stack_t uc_stack;
}coctx;

extern "C" {
extern void swap_ctx(coctx *src, coctx *dst) asm("swap_ctx");
};

extern "C" {
extern void start_ctx() asm("start_ctx");
};

extern "C" {
extern void __set_ctx(coctx *dst) asm("__set_ctx");
};

void make_ctx(coctx *ctx, co_start func, void *s);

#endif //COROUTINE_COCTX_H