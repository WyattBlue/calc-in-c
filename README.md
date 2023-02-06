### C-Calc

Example usage:

```bash
> 4 + 10
14
> 34 + 5 * 3
49
> 5 / 4
1
```

### Compiling on MacOS
Compile and run with clang and link libedit, a library included in MacOS.

```
cc -ledit calc.c && ./a.out
```

### Compiling on Other Operating Systems
Any decent C compiler should work. Make sure you link a library that's compitable with readline.

