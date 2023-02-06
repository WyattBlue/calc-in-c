### C-Calc

Calc-in-C is a command line calculator that can add, subtract, multiply and divide.

Features:
 - Correct order of operations
 - Parentheses to change ordering
 - REPL with history (via readline)
 - Error recovery

Example usage:

```bash
> 4 + 10
14
> 34 + 5 * 3
49
>20 * (10 + 4)
280
> 5 / 4
1
```

### Compiling on MacOS
Compile and run with clang and link libedit, a library included in MacOS.

```
cc -ledit calc.c && ./a.out
```

### Compiling on Other Operating Systems
Any decent C compiler should work. Make sure you link a library that's compatible with readline.

