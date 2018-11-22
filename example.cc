
#include <iostream>

#include "scope_timer.h"


int fib(int i) {
    if (i <= 1)
        return i;
    else
        return (fib(i - 1) + fib(i - 2)) % 100;
}

volatile int foo = 0;

int main()
{
    TIME_SCOPE_SEC(elapsed_seconds){
        std::cout << "Main took: " << elapsed_seconds << "seconds" << std::endl;
    };

    double total = 0.0;
    for (int i = 0; i < 10; ++i) {
        TIME_SCOPE_MILLI(elapsed){ total += elapsed; };
        foo += fib(39);
    }

    std::cout << "fib(39) took on average: " << total / 10 << " ms" << std::endl;
}
