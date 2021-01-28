//
// Created by xclwt on 2021/1/28.
//

.globl start_ctx
.globl __set_ctx
.type  start_ctx, @function
start_ctx:
    //movq %rbx, %rsp
    movq (%rsp), %rdi

    call __set_ctx

__set_ctx:
    movq 32(%rdi), %rbp
    movq 64(%rdi), %rsp
    movq (%rdi), %r15
    movq 8(%rdi), %r14
    movq 16(%rdi), %r13
    movq 24(%rdi), %r12
    movq 40(%rdi), %rdi
    movq 56(%rdi), %rbx

    leaq 8(%rsp), %rsp
    pushq 48(%rdi)

    ret