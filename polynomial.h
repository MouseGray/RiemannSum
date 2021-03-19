#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include "function.h"

class Polynomial : public QObject
{
    Q_OBJECT
public:
    Polynomial();

    Function::cType operator() (const Function::cType& x);

    void setA(double value) { A = value; update(); }
    void setB(double value) { B = value; update(); }
    void setN(int value)    { N = value; update(); }

    void setF(Function *value);

public slots:
    void functionChanged();
private:
    void update();

    double A = -100.0;
    double B =  100.0;

    int N = 10;

    Function* f;

    std::vector<Function::cType> finiteDiff;
};

#endif // POLYNOMIAL_H
