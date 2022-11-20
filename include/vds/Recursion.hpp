#pragma once

#include <type_traits>
#include <iterator>

namespace Recursion {

int factorial(int n); 
int fibonacci(int n);

template <typename It>
auto sum(It begin, It end) {
    using ValueType = typename std::iterator_traits<It>::value_type;
    if (begin == end) return ValueType{};
    return *begin + sum(begin + 1, end);
}

template <typename It>
auto sum_tail(
    It begin,
    It end,
    typename std::iterator_traits<It>::value_type accumulator = typename std::iterator_traits<It>::value_type{})
{
    if (begin == end) return accumulator;
    return sum_tail(begin + 1, end, accumulator + *begin);
}


}