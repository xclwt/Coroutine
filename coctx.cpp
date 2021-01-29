//
// Created by xclwt on 2021/1/28.
//

#include "coctx.h"

void make_ctx(coctx *ctx, co_start func, void *s){
    auto *sp = (long long *)((uintptr_t)ctx->uc_stack.ss_sp + ctx->uc_stack.ss_size);

    /* 16 Bytes room reserved for uc_link and start_ctx addr*/
    sp -= 2;

    /* Align stack, 8 Bytes room reserved for swap_ctx ret addr */
    sp = (long long *)(((uintptr_t)sp & -16L) - 8);

    ctx->regs[RSP] = (char *)sp;
    ctx->regs[RBX] = (char *)&sp[2];
    ctx->regs[RDI] = (char *)s;
    ctx->regs[RET] = (char *)func;

    sp[1] = (uintptr_t)start_ctx;
    if (ctx->uc_link)
        sp[2] = (uintptr_t)ctx->uc_link;
    else
        sp[2] = (uintptr_t)func;
}

