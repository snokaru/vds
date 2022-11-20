#include "vds/Recursion.hpp"

namespace Recursion {

int factorial(int n)
{
    if (n == 1) return 1;
    return factorial(n - 1) * n;
}

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

}