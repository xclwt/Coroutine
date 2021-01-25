# Coroutine

### Abstract

A simple coroutine lib implemented by C++, with reference to [@cloudwu](https://github.com/cloudwu)'s [coroutine](https://github.com/cloudwu/coroutine).

### TODO

- Optimize the performance of switching ucontext.
- Add coroutine with independent runtime stack.
- Try to add support to Block Syscall(maybe just read() and write()).

### Note

- As the coroutines use shared runtime stack, so don't use address on stack as buffer in async call! When one coroutine is suspended, the buffer on stack don't belong to it any more!

- Don't use Block Syscall in coroutine.

