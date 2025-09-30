#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMA ,
#define __EXEC(code)                                                           \
  do {                                                                         \
    code                                                                       \
  } while (0)

typedef struct {
  void *val;
  size_t size;
} Arg;

typedef struct {
  void (*fun)(Arg *__args, int __argc, void *__out);
  size_t argCount;
} Function;

typedef struct {
  Function *fun;
  Arg *args;
  size_t len;
} CurriedFunction;

#define ERROR(msg) __EXEC(fprintf(stderr, "%s\n", msg); abort();)

#define FUNCTIONx(__argCount, __name, __rtype, __rname, __impl)                \
  void __##__name(Arg *__args, int __argc, void *__out) {                      \
    if (__argc != __argCount) {                                                \
      ERROR("Bad arg count for function");                                     \
    }                                                                          \
    __rtype __rname;                                                           \
    __EXEC(__impl);                                                            \
    *(__rtype *)__out = __rname;                                               \
  }                                                                            \
  Function __name = {&__##__name, __argCount};

#define FUNCTION1(__name, __type1, __arg1, __impl, __rtype, __rname)           \
  FUNCTIONx(1, __name, __rtype, __rname, {                                     \
    if (__args[0].size != sizeof(__type1)) {                                   \
      ERROR("Bad arg1 size for function");                                     \
    }                                                                          \
    __type1 __arg1 = *(__type1 *)__args[0].val;                                \
    __EXEC(__impl);                                                            \
  })

#define FUNCTION2(__name, __type1, __arg1, __type2, __arg2, __impl, __rtype,   \
                  __rname)                                                     \
  FUNCTIONx(2, __name, __rtype, __rname, {                                     \
    if (__args[0].size != sizeof(__type1)) {                                   \
      ERROR("Bad arg1 size for function");                                     \
    }                                                                          \
    __type1 __arg1 = *(__type1 *)__args[0].val;                                \
    if (__args[1].size != sizeof(__type2)) {                                   \
      ERROR("Bad arg2 size for function");                                     \
    }                                                                          \
    __type2 __arg2 = *(__type2 *)__args[1].val;                                \
    __EXEC(__impl);                                                            \
  })

#define FUNCTION3(__name, __type1, __arg1, __type2, __arg2, __impl, __rtype,   \
                  __rname)                                                     \
  FUNCTIONx(3, __name, __rtype, __rname, {                                     \
    if (__args[0].size != sizeof(__type1)) {                                   \
      ERROR("Bad arg1 size for function");                                     \
    }                                                                          \
    __type1 __arg1 = *(__type1 *)__args[0].val;                                \
    if (__args[1].size != sizeof(__type2)) {                                   \
      ERROR("Bad arg2 size for function");                                     \
    }                                                                          \
    __type2 __arg2 = *(__type2 *)__args[1].val;                                \
    if (__args[1].size != sizeof(__type3)) {                                   \
      ERROR("Bad arg3 size for function");                                     \
    }                                                                          \
    __type3 __arg3 = *(__type3 *)__args[2].val;                                \
    __EXEC(__impl);                                                            \
  })

#define INIT_CURRY(__func) {&__func, malloc(__func.argCount * sizeof(Arg)), 0}
#define INIT_CURRYN(__n) {NULL, malloc(__n * sizeof(Arg)), 0}

#define FREE_CURRY(__cfunc) __EXEC(free(__cfunc.args);)

#define COPY_CFUNC(__dest, __src)                                              \
  __EXEC(__dest.fun = __src.fun; __dest.len = __src.len;                       \
         memcpy(__dest.args, __src.args, __src.len * sizeof(Arg));)

// Curries the CurriedFunction, creating its new copy and leaving it as is
#define CURRY_NEW(__curry, __cfunc, __val)                                     \
  __EXEC(COPY_CFUNC(__curry, __cfunc); CURRY_INPLACE(__curry, __val);)

// Curries the CurriedFunction, modifying its args list
#define CURRY_INPLACE(__cfunc, __val)                                          \
  __EXEC(if (__cfunc.len == __cfunc.fun->argCount) {                           \
    ERROR("Trying to curry with more params than function accepts");           \
  } __cfunc.args[__cfunc.len]                                                  \
             .val = __val;                                                     \
         __cfunc.args[__cfunc.len].size = sizeof(*__val); __cfunc.len++;)
// Call the CurriedFunction
#define CALL(__cfun, __out)                                                    \
  __EXEC(__cfun.fun->fun(__cfun.args, __cfun.len, __out);)

// Call the CurriedFunction, adding 1 remaining arg to it.
#define CALL1(__cfun, __param1, __out)                                         \
  CurriedFunction _tmp = INIT_CURRYN(__cfun.fun->argCount);                    \
  CURRY_NEW(_tmp, __cfun, __param1);                                           \
  CALL(_tmp, __out);                                                           \
  FREE_CURRY(_tmp);

#define CALL1_INPLACE(__cfun, __param1, __out)                                 \
  CURRY_INPLACE(__cfun, __param1);                                             \
  CALL(__cfun, __out);

// Call the CurriedFunction, adding 2 remaining args to it.
#define CALL2(__cfun, __param1, __param2, __out)                               \
  CurriedFunction _tmp2 = INIT_CURRYN(__cfun.fun->argCount);                   \
  CURRY_NEW(_tmp2, __cfun, __param1);                                          \
  CALL1_INPLACE(_tmp2, __param2, __out);                                       \
  FREE_CURRY(_tmp2);

#define CALL2_INPLACE(__cfun, __param1, __param2, __out)                       \
  CURRY_INPLACE(__cfun, __param1);                                             \
  CALL1_INPLACE(__cfun, __param2, __out);

// Call the CurriedFunction, adding 2 remaining args to it.
#define CALL3(__cfun, __param1, __param2, __param3, __out)                     \
  CurriedFunction _tmp3 = INIT_CURRYN(__cfun.fun->argCount);                   \
  CURRY_NEW(_tmp3, __cfun, __param1);                                          \
  CALL2_INPLACE(_tmp3, __param2, __param3, __out);                             \
  FREE_CURRY(_tmp3);
