//
// Created by xclwt on 2021/1/28.
//

#include "coctx.h"

void make_ctx(coctx *ctx, co_start func, void *s){
    //extern void start_ctx() asm("start_ctx");

    auto *sp = (long long *)((uintptr_t)ctx->uc_stack.ss_sp + ctx->uc_stack.ss_size);

    /* 8 Bytes room reserved for uc_link */
    sp -= 1;
    /* Align stack, 8 Bytes room reserved for __start_context */
    sp = (long long *)(((uintptr_t)sp & -16L) - 8);

    ctx->regs[RSP] = (char *)sp;
    ctx->regs[RBX] = (char *)&sp[1];
    ctx->regs[RDI] = (char *)s;
    ctx->regs[RET] = (char *)func;

    sp[0] = (uintptr_t)start_ctx;
    if (ctx->uc_link)
        sp[1] = (uintptr_t)ctx->uc_link;
    else
        sp[1] = (uintptr_t)func;
}

