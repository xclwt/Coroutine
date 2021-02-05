//
// Created by xclwt on 2021/1/28.
//

.globl start_ctx
.globl __set_ctx
.type  start_ctx, @function
start_ctx:
    movq (%rsp), %rdi

    call __set_ctx

__set_ctx:
    movq 32(%rdi), %rbp
    movq 96(%rdi), %rsp
    movq (%rdi), %r15
    movq 8(%rdi), %r14
    movq 16(%rdi), %r13
    movq 24(%rdi), %r12
    movq 48(%rdi), %rcx
    movq 56(%rdi), %rdx
    movq 64(%rdi), %r8
    movq 72(%rdi), %r9
    movq 88(%rdi), %rbx

    leaq 8(%rsp), %rsp
	pushq 80(%rdi)
	
	#movq 40(%rdi), %rdi

    ret
