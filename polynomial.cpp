#include "polynomial.h"

#include <algorithm>

Polynomial::Polynomial()
{
}

Function::cType Polynomial::operator()(const Function::cType &x)
{
    assert(f != nullptr && "Function not set");
    assert(N != 0 && "0 interpolation nodes");
    assert(finiteDiff.size() >= static_cast<size_t>(N) && "Finite differences not updated");

    auto result = (*f)(A);
    auto h = double(B-A)/(N);

    Function::cType q = (x - A)/h;
    Function::cType accum = 1.0;

    for(auto k = 1; k < N + 1; k++){
        accum *= (q - k + 1)/k;
        result += accum*finiteDiff[k - 1];
    }

    return result;
}

void Polynomial::setF(Function *value)
{
    assert(value != nullptr);

    f = value;

    connect(f, &Function::changed, this, &Polynomial::functionChanged, Qt::DirectConnection);

    update();
}

void Polynomial::functionChanged()
{
    update();
}

void Polynomial::update()
{
    std::vector<Function::cType> fvec(N + 1, 0.0);
    std::vector<Function::cType> svec(N + 1, 0.0);

    std::for_each(fvec.begin(), fvec.end(),
                  [this, x = A, h = (B-A)/N]
                  (Function::cType& el) mutable { el = (*f)(x); x += h; });

    finiteDiff.clear();

    for(auto j = 0; j < N; j++) {
        for(auto i = 0; i < N - j; i++)
            svec[i] = fvec[i + 1] - fvec[i];

        finiteDiff.push_back(svec[0]);
        std::swap(fvec, svec);
    }
}
