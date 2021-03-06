//
// Created by xclwt on 2021/1/28.
//

.globl swap_ctx
.globl change_stack
.type  swap_ctx, @function

swap_ctx:
	leaq (%rsp),%rax
    movq %rax, 64(%rdi)

    movq %rbx, 56(%rdi)

	movq 0(%rax), %rax
	movq %rax, 48(%rdi)

	#movq %rdi, 40(%rdi)
    movq %rbp, 32(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 16(%rdi)
    movq %r14, 8(%rdi)
    movq %r15, (%rdi)
	xorq %rax, %rax

    movq 32(%rsi), %rbp
    movq 64(%rsi), %rsp
    movq (%rsi), %r15
    movq 8(%rsi), %r14
    movq 16(%rsi), %r13
    movq 24(%rsi), %r12
    movq 40(%rsi), %rdi
    movq 56(%rsi), %rbx

    leaq 8(%rsp), %rsp
	pushq 48(%rsi)

	ret
