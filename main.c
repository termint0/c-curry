#include "curry.h"
FUNCTION1(
    // Define a function `foo 1`
    foo1,
    // Which takes a `long` arg called `val`
    long, val,
    // Executes this code
    {
      // code
      val2 = 1 + val;
    },
    // And puts the value of the variable long val2 into the output parameter
    long, val2)

FUNCTION2(
    lmultiply,
    // arg 1
    long, a,
    // arg 2
    long, b,
    // implementation
    {
      // code
      mul = a * b;
    },
    // return type
    long, mul)

int main(int argc, char *argv[]) {
  { // 1 arg curry
    CurriedFunction curry = INIT_CURRY(foo1);
    long a = 10l;
    long b = 20l;
    long result;
    CurriedFunction newCurry = INIT_CURRY(foo1);
    CURRY_NEW(newCurry, curry, b);
    CURRY_INPLACE(curry, a);
    CALL(curry, &result);
    printf("result: %ld\n", result);
    CALL(newCurry, &result);
    printf("result: %ld\n", result);
    FREE_CURRY(newCurry);
    FREE_CURRY(curry);
  }
  { // 2 args
    int result;
    CurriedFunction curry = INIT_CURRY(lmultiply);
    int a = 10;

    CALL2(curry, a, a, &result);
    printf("Result %d\n", result);

    CURRY_INPLACE(curry, a);
    for (int i = 0; i < 11; ++i) {
      CALL1(curry, i, &result);
      printf("Result %d\n", result);
    }
    FREE_CURRY(curry);
  }
  return EXIT_SUCCESS;
}
