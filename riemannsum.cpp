#include "riemannsum.h"

#include <algorithm>
#include <QDebug>

Function::cType RiemannSum::operator()(const Function::cType &fix)
{
    auto first_n = N;
    auto second_n = 2*N;
    auto first = calculate(fix, first_n);
    auto second = calculate(fix, second_n);
    while(std::abs(first - second)/3 > d) {
        first_n = second_n;
        second_n = 2*second_n;

        first = calculate(fix, first_n);
        second = calculate(fix, second_n);
    }

    return first;
}

void RiemannSum::setF(Function *value)
{
    assert(value != nullptr);
    f = value;
}

Function::cType RiemannSum::calculate(const Function::cType &fix, int n)
{
    auto h = (B - A)/n;
    auto result = 0;
    for (auto i = 1; i < n + 1; i ++) {
        result += (*f)(A + i*h/2, fix, fixType);
    }
    return result*h;
}
