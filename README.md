# Coroutine

### Abstract


A coroutine library implemented by C++, supporting both coroutine with shared runtime stack and coroutine with independent runtime stack.

### Table of Contents


[TOC]
### API

#### Schedule

The first method creates a schedule with a 1MB runtime stack. All shared-stack coroutines created by the schedule will share this 1MB runtime stack.

The second method creates a schedule with a runtime stack whose size is determined by `stack_size`.

#### coroutine_create

```c++
int Schedule::coroutine_create(co_func func, void *args);

int Schedule::coroutine_create(co_func func, void *args, int stack_type, int stack_size);
```

The first method creates a coroutine with shared runtime stack, it equals to: `Schedule::coroutine_create(func, args, SAVED_STACK, 0);`

The second method creates a coroutine with shared runtime stack when `stack_type` is `SAVED_STACK`. `stack_size` determines the initial size of the memory saving coroutine's runtimestack, and the memory will be dynamically reallocated when the coroutine is suspended, so `0` is recomended for `stack_size`.

The second method creates a coroutine with independent runtime stack when `stack_type` is `INDEPENDENT_STACK`. `stack_size` determines the size(Bytes) of independent runtime stack.

#### coroutine_resume

```c++
void Schedule::coroutine_resume(int co_id);
```

This method switches the main coroutine to the non-main coroutine appointed by co_id.

#### coroutine_yield

```c++
void Schedule::coroutine_yield();
```

This method suspends the running non-main coroutine and switches to main coroutine.

#### coroutine_status


```c++
int Schedule::coroutine_status(int co_id);
```

This method returns the running coroutine status. The status(defined in `coroutine.h`) can be `COROUTINE_DEAD`, `COROUTINE_READY`, `COROUTINE_RUNNING`, `COROUTINE_SUSPEND`.

#### coroutine_running


```c++
int Schedule::coroutine_running() const;
```

This method returns the running coroutine id.

### Testing Example

The test example is in test.cpp, and you can get more output of testing information by `#define COROUTINE_TEST_OUTPUT`in `coroutine.h`.

### TODO

- [x] Add coroutine with independent runtime stack.

- [ ] Optimize the performance of switching ucontext.

- [ ] Try to add support to Block Syscall(maybe just read() and write()).


### Suggestions

- If the coroutines use shared runtime stack, don't use address on stack as buffer in async call! When one coroutine is suspended, the buffer allocated from runtime stack doesn't belong to it any more!

- It's better to use Unblock Syscall than Block Syscall in coroutine.

### Reference

- [cloudwu/coroutine](https://github.com/cloudwu/coroutine)
- [Tencent/libco](https://github.com/Tencent/libco) 