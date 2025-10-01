### Currying for C
This is a set of C macros that make it easy to use currying (or rather partial application) of functions in C. This code was made as a personal fun project and shouldn't be used in production without a thorough review of the source code.

### Use
#### Declaring a function
To define a curryable function, use the `FUNCTION<n>` macro. The return variable will be declared
before the function code. Instead of returning, assing the return value to it.
```c
FUNCTION<n>(
    <function name>,
    <type of arg1>, <arg1>,
    // ... more args
    {
      // function code
    },
    <return type>, <return variable>
)
```
for example, this is a function for multiplication of two longs.
```c
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
    long, mul
)
```

#### Currying
Initialize a curried function using `INIT_CURRY(<foo>)` (don't forget to call `FREE_CURRY` for every result of `INIT_CURRY`). To bind an arg to this `CurriedFunction`, use `CURRY_INPLACE(<cfoo>, <arg>)`. To bind an arg to a new `CurriedFunction` and leave `<cfoo>` intact, use `CURRY_NEW(<newCfoo>, <cfoo>, <arg>)`. Note that `<newCfoo>` needs to be initialized for the number of arguments, not doing so is undefined behavior. 
```c
CurriedFunction curry = INIT_CURRY(lmultiply);
long a = 10;
// curry = (long b) => 10 * b
CURRY_INPLACE(curry, a);

long b = 20;
// init curry2 for 2 params;
CurriedFunction curry2 = INIT_CURRYN(2) // or INIT_CURRY(lmultiply);

// curry2 = () => 10 * 20
CURRY_NEW(curry2, curry, b);

// trying to curry with 3 args for a function with 2 args
// will call abort()
CURRY_INPLACE(curry2, b);
FREE_CURRY(curry);
FREE_CURRY(curry2);
```
#### Calling
If a function has all of its args bound, it can be called using the `CALL` macro. For convenience, there is the `CALL<n>(cfoo, arg_1, ..., arg_n)` macro, which binds `<n>` args to the function and calls it, leaving `cfoo` intact.
```c
long result;
CurriedFunction curry = INIT_CURRY(lmultiply);

long a = 10;

CALL2(curry, a, a, &result);
// result == 100
// curry == (long a, long b) => a * b

// curry = (long b) => 10 * b
CURRY_INPLACE(curry, a);

long b = 20;
CALL1(curry, b, &result); // calls abort(); wrong arg count;
// result == 200

CALL2(curry, a, a, &result); // calls abort(); wrong arg count;

// curry = () => 10 * 20
CURRY_INPLACE(curry, b);

CALL(curry, &result)
// result == 200
FREE_CURRY(curry);
```
