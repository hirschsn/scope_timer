# Scope Timer

Times a scope and executes a block of code with the elapsed time as parameter
at the end of the scope.

## Example

```c++
#include <iostream>
#include "scope_timer.h"

long fib(long i) { if (i <= 1) return i; else return fib(i - 1) + fib(i - 2); }

int main()
{
    TIME_SCOPE_SEC(elapsed_seconds){
        std::cout << "Main took: " << elapsed_seconds << "seconds" << std::endl;
    };

    double total = 0.0;
    for (int i = 0; i < 10; ++i) {
        TIME_SCOPE_MILLI(elapsed){ total += elapsed; };
        printf("Fib(39) = %li\n", fib(39));
    }

    std::cout << "fib(39) took on average: " << total / 10 << " ms" << std::endl;
}
```

## Acknoweldgements

Acknowledgements go to Andrei Alexandrescu as this code uses his scope guard
concept and his trick for their syntactic abstraction with lambdas.
