//
// Created by xclwt on 2021/1/28.
//

.globl swap_ctx
.type  swap_ctx, @function

swap_ctx:
	leaq (%rsp),%rax
    movq %rax, 96(%rdi)

    movq %rbx, 88(%rdi)

	movq 0(%rax), %rax
	movq %rax, 80(%rdi)

	movq %r9, 72(%rdi)
	movq %r8, 64(%rdi)
	movq %rdx, 56(%rdi)
	movq %rcx, 48(%rdi)
	#movq %rdi, 40(%rdi)
    movq %rbp, 32(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 16(%rdi)
    movq %r14, 8(%rdi)
    movq %r15, (%rdi)
	xorq %rax, %rax

    movq 32(%rsi), %rbp
    movq 96(%rsi), %rsp
    movq (%rsi), %r15
    movq 8(%rsi), %r14
    movq 16(%rsi), %r13
    movq 24(%rsi), %r12
    movq 40(%rsi), %rdi
    movq 48(%rsi), %rcx
    movq 56(%rsi), %rdx
    movq 64(%rsi), %r8
    movq 72(%rsi), %r9
    movq 88(%rsi), %rbx

    leaq 8(%rsp), %rsp
	pushq 80(%rsi)

	ret
	
