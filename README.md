### Currying for C
This is a set of C macros that make it easy to use currying (or rather partial application) of functions in C.

### Use
#### Declaring a function
To define a curryable function, use the FUNCTION<n> macro. The return variable will be declared
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
```


