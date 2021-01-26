# Coroutine

### Abstract

A simple coroutine lib implemented by C++, with reference to [@cloudwu](https://github.com/cloudwu)'s [coroutine](https://github.com/cloudwu/coroutine).

### TODO

- [x] Add coroutine with independent runtime stack.
- [ ] Optimize the performance of switching ucontext.
- [ ] Try to add support to Block Syscall(maybe just read() and write()).

### Note

- If the coroutines use shared runtime stack, don't use address on stack as buffer in async call! When one coroutine is suspended, the buffer allocated from runtime stack doesn't belong to it any more!

- It's better to use Unblock Syscall than Block Syscall in coroutine.

