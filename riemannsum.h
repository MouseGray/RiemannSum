#ifndef RIEMANNSUM_H
#define RIEMANNSUM_H

#include <QObject>

#include "function.h"

class RiemannSum : public QObject
{
    Q_OBJECT
public:
    RiemannSum() {}

    Function::cType operator ()(const Function::cType& fix);

    void setA(double value) { A = value; }
    void setB(double value) { B = value; }
    void setN(int value)    { N = value; }

    void setd(int value)    { d = value; }

    void setF(Function *value);

    void setFixType(Coefficient type) { fixType = type; }
private:
    Function::cType calculate(const Function::cType& fix, int n);

    Function* f;

    double A = -100.0;
    double B =  100.0;
    double d =  0.1;

    int N = 10;

    Coefficient fixType = Coefficient::Gamma;
};

#endif // RIEMANNSUM_H
